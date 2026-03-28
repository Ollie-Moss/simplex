#pragma once

// UITree.h
//
// Replaces UIBuilder.h and the entity-management parts of UIStateSystem.
//
// UITree owns a single root EntityId and knows how to:
//   1. Mount  – create entities from a UINode tree for the first time.
//   2. Reconcile – diff a NEW UINode tree against the live entity tree and
//                  apply only the minimal set of changes (update props in-place,
//                  create missing children, destroy removed children, re-order).
//
// ── Key-based reconciliation ──────────────────────────────────────────────────
//
//   Each UINode carries a `props.key` string.  During reconciliation the
//   algorithm matches old children to new children by key (falling back to
//   positional index when key is empty, exactly like React without keys).
//
//   This means:
//     • Stable nodes have their ECS components patched in-place  → no alloc.
//     • Removed nodes have their entire subtree destroyed.
//     • New nodes are mounted fresh.
//     • Re-ordered nodes are correctly identified and NOT spuriously destroyed.
//
// ── Dynamic children ─────────────────────────────────────────────────────────
//
//   If a UINode has a ChildrenFn the tree stores it on the UIElement so
//   UIStateSystem can call it every frame.  UIStateSystem no longer needs to
//   know about specs at all — it just calls UITree::ReconcileChildren().
//
// ── Thread safety ────────────────────────────────────────────────────────────
//
//   The same as the rest of the ECS: single-threaded, driven by the system
//   update loop.

#include "gui/UINode.h"
#include "gui/UIComponents.h"
#include "gui/Text.h"
#include "core/Entity.h"
#include "core/Registry.h"
#include "core/Types.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// UIEntityMeta – stored alongside each UIElement to support reconciliation.
// We store the last-seen UINodeProps so we can skip ECS writes when nothing
// changed, and a copy of the ChildrenFn when children are dynamic.
// ─────────────────────────────────────────────────────────────────────────────

struct UIEntityMeta
{
    // The props that were used to create / last update this entity.
    // If the new UINode's props differ, we patch the ECS components.
    UINodeProps lastProps;

    // Non-null when this node was built with WithDynamicChildren().
    // UIStateSystem calls this each frame and reconciles the result.
    ChildrenFn dynamicChildrenFn;

    bool hasDynamicChildren = false;
};

// ─────────────────────────────────────────────────────────────────────────────
// UITree
// ─────────────────────────────────────────────────────────────────────────────

class UITree
{
  public:
    // ── Public API ───────────────────────────────────────────────────────────

    // Build the initial entity tree from a root UINode.
    // Call once, store the UITree, then call Reconcile() every frame.
    EntityId Mount(Registry &registry, const UINode &root)
    {
        m_Registry = &registry;
        m_Root = MountNode(root, NULL_ENTITY);
        return m_Root;
    }

    // Diff `newRoot` against the live tree rooted at m_Root and patch
    // only what changed.  Returns the root EntityId (unchanged).
    EntityId Reconcile(const UINode &newRoot)
    {
        ReconcileNode(m_Root, newRoot);
        return m_Root;
    }

    // Called by UIStateSystem for nodes that have dynamic children.
    // `entity` must be a live entity with UIEntityMeta.hasDynamicChildren == true.
    void ReconcileDynamicChildren(EntityId entity)
    {
        UIEntityMeta &meta = m_Registry->GetComponent<UIEntityMeta>(entity);
        if(!meta.hasDynamicChildren || !meta.dynamicChildrenFn)
            return;

        std::vector<UINode> newChildren = meta.dynamicChildrenFn();
        ReconcileChildList(entity, newChildren);
    }

    EntityId GetRoot() const { return m_Root; }

  private:
    Registry *m_Registry = nullptr;
    EntityId m_Root = NULL_ENTITY;

    // ── Mount ────────────────────────────────────────────────────────────────

    EntityId MountNode(const UINode &node, EntityId parent)
    {
        Entity entity = m_Registry->QueueCreate<
            UITransform, UILayout, UIStyle, TextLayout, ::Text, UIEvents, UIEntityMeta>(
            {},
            node.props.layout,
            node.props.style,
            {},
            node.props.text,
            node.props.events,
            UIEntityMeta{.lastProps = node.props});

        // Attach any extra components the caller registered
        for(auto &attach : node.extraComponents)
            attach((EntityId)entity);

        // Build children and wire up UIElement
        std::vector<EntityId> childIds = MountChildren(node, entity);

        // Store dynamic fn if present
        UIEntityMeta &meta = m_Registry->GetComponent<UIEntityMeta>((EntityId)entity);
        if(node.HasDynamicChildren())
        {
            meta.hasDynamicChildren = true;
            meta.dynamicChildrenFn = node.GetChildrenFn();
        }

        m_Registry->QueueComponent<UIElement>(
            entity,
            UIElement{
                .id = node.props.key,
                .parent = parent,
                .children = childIds,
                .dirty = true,
            });

        std::cout << "[UITree] Mount entity=" << (int)(EntityId)entity
                  << " key=\"" << node.props.key << "\""
                  << " children=" << childIds.size() << "\n";

        return entity;
    }

    std::vector<EntityId> MountChildren(const UINode &node, EntityId parent)
    {
        std::vector<EntityId> ids;

        if(node.HasDynamicChildren())
        {
            // Evaluate the fn once at mount time so the initial frame is populated
            std::vector<UINode> initial = node.GetChildrenFn()();
            for(auto &child : initial)
                ids.push_back(MountNode(child, parent));
        }
        else
        {
            for(auto &child : node.GetStaticChildren())
                ids.push_back(MountNode(child, parent));
        }

        return ids;
    }

    // ── Reconcile ────────────────────────────────────────────────────────────

    // Patch an existing entity in-place to match `newNode`.
    void ReconcileNode(EntityId entity, const UINode &newNode)
    {
        UIEntityMeta &meta = m_Registry->GetComponent<UIEntityMeta>(entity);

        // ── Patch props if changed ────────────────────────────────────────────
        if(meta.lastProps != newNode.props)
        {
            PatchProps(entity, newNode.props);
            meta.lastProps = newNode.props;

            // Mark dirty so the layout system recalculates
            UIElement &elem = m_Registry->GetComponent<UIElement>(entity);
            elem.dirty = true;
        }

        // ── Update dynamic fn ────────────────────────────────────────────────
        if(newNode.HasDynamicChildren())
        {
            meta.hasDynamicChildren = true;
            meta.dynamicChildrenFn = newNode.GetChildrenFn();
            // Actual child reconciliation is done by UIStateSystem each frame
        }
        else
        {
            meta.hasDynamicChildren = false;
            meta.dynamicChildrenFn = nullptr;
            ReconcileChildList(entity, newNode.GetStaticChildren());
        }
    }

    // Diff `newChildren` against the current children of `parentEntity`.
    void ReconcileChildList(EntityId parentEntity, const std::vector<UINode> &newChildren)
    {
        UIElement &elem = m_Registry->GetComponent<UIElement>(parentEntity);

        // ── Build key → entityId map from current children ────────────────────
        // Positional children (empty key) are keyed by "~idx"
        std::unordered_map<std::string, EntityId> oldByKey;
        for(size_t i = 0; i < elem.children.size(); ++i)
        {
            EntityId childId = elem.children[i];
            std::string key = GetKey(childId, i);
            oldByKey[key] = childId;
        }

        // ── Match new children to old ones ────────────────────────────────────
        std::vector<EntityId> nextChildren;
        nextChildren.reserve(newChildren.size());

        std::unordered_map<std::string, bool> usedKeys;

        for(size_t i = 0; i < newChildren.size(); ++i)
        {
            const UINode &newChild = newChildren[i];
            std::string key = !newChild.props.key.empty()
                                  ? newChild.props.key
                                  : ("~" + std::to_string(i));

            auto it = oldByKey.find(key);
            if(it != oldByKey.end())
            {
                // Existing node: reconcile in-place
                ReconcileNode(it->second, newChild);
                nextChildren.push_back(it->second);
                usedKeys[key] = true;
            }
            else
            {
                // New node: mount fresh
                EntityId newId = MountNode(newChild, parentEntity);
                nextChildren.push_back(newId);
                std::cout << "[UITree] New child key=\"" << key
                          << "\" mounted under parent=" << (int)parentEntity << "\n";
            }
        }

        // ── Destroy removed children ─────────────────────────────────────────
        for(size_t i = 0; i < elem.children.size(); ++i)
        {
            EntityId childId = elem.children[i];
            std::string key = GetKey(childId, i);
            if(!usedKeys.count(key))
            {
                std::cout << "[UITree] Destroy removed child key=\"" << key
                          << "\" entity=" << (int)childId << "\n";
                DestroyTree(childId);
            }
        }

        // ── Detect reordering and mark parent dirty ───────────────────────────
        if(nextChildren != elem.children)
        {
            elem.children = nextChildren;
            elem.dirty = true;
        }
    }

    // ── Prop patching ────────────────────────────────────────────────────────

    void PatchProps(EntityId entity, const UINodeProps &props)
    {
        m_Registry->GetComponent<UILayout>(entity) = props.layout;
        m_Registry->GetComponent<UIStyle>(entity) = props.style;
        m_Registry->GetComponent<::Text>(entity) = props.text;
        m_Registry->GetComponent<UIEvents>(entity) = props.events;
    }

    // ── Helpers ──────────────────────────────────────────────────────────────

    // Return the reconciliation key for a live entity.
    // Uses UIElement.id if set, otherwise falls back to positional "~N".
    std::string GetKey(EntityId entity, size_t position) const
    {
        const UIElement &elem = m_Registry->GetComponent<UIElement>(entity);
        if(!elem.id.empty() && elem.id != "N/A")
            return elem.id;
        return "~" + std::to_string(position);
    }

    void DestroyTree(EntityId entity)
    {
        UIElement &elem = m_Registry->GetComponent<UIElement>(entity);
        for(EntityId child : elem.children)
            DestroyTree(child);
        elem.children.clear();
        m_Registry->Destroy(entity);
    }
};
