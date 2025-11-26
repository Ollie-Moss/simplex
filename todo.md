# Todo

## Overview
- [x] UI
- [x] Renderer refactor (Make renderer support 'modules' for different aspects of rendering, e.g. sprite renderer, line renderer, etc)
- [] Basic components and systems e.g. physics, character controller, etc
- [] Audio
- [] Refactor resource management
- [] Scene mangement
- [] Optimize ECS with sparse sets, etc if required

## Backlog
- [] Texture atlas support & spritesheet tool
- [] Editor for scene, and resource management
- [] Networking

## UI
- [] Space-between and space around for justify-content
- [x] Text support for elements
- [x] Text wrapping
- [] Input handling
- [] Input field
- [] Image element
- [] Button element
- [] Checkbox element
- [] Slider element
- [] Overflow handling
- [] Scroll areas
- [-] Data binding
- [] Developer user interface for debugging

## 2D Physics

### Overview

- [x] Components (Transform → Rigidbody → Collider)
- [x] Integration system
- [ ] Broad-phase
- [ ] Narrow-phase
- [ ] Resolution (position + velocity)
- [ ] Friction
- [ ] Triggers & events
- [ ] System ordering
- [ ] Debug visualization

### Components 

- [x] Transform2D (position, rotation, scale)

- [x] Rigidbody2D (mass, velocity, forces, static/dynamic flag)

- [x] Collider2D (shape type, size parameters, offset)

- [*] (Optional later) Physics material (friction, bounciness)

### Physics Integration System

- [x] Reading forces from Rigidbody2D
- [x] Converting forces into acceleration
- [x] Updating velocity
- [x] Integrating velocity into the Transform position
- [x] Clearing forces afterward

### Broad-Phase Collision Detection

Quickly figure out which pairs of objects might be colliding.

- [ ] Creating a world-space bounding box for every collider
- [ ] Using a simple method first (e.g., pairwise AABB overlap tests)

Later upgrade to something like spatial hashing or a quadtree to scale better

Output: a list of potential collision pairs that need accurate testing.

### Narrow-Phase Collision Detection

For every pair found above:
Use mathematically precise tests for the shapes you care about

- Circle vs circle
- AABB vs AABB
- Circle vs AABB

(Later: oriented boxes, polygons, etc.)

This step must also:
- Provide the collision normal
- Provide the penetration depth
- Optionally determine a contact point

These are required for resolution in the next step.

### Collision Resolution

This stage has two subparts:

#### Positional Correction

- [ ] Move bodies so they are no longer overlapping.
- [ ] Use the penetration depth and collision normal, distributing movement based on their masses (static body = no movement).

#### Velocity/Impulse Correction

- [ ] Adjust velocities to simulate proper bouncing and sliding:
- [ ] Compute relative velocity along the collision normal
- [ ] Apply an impulse so bodies move apart
- [ ] Use restitution/bounciness if you add physics materials

After this step, objects won’t stick, pass through each other, or jitter excessively.

### Add Friction (Optional but common)

Once impulse resolution works, handle friction by:

- [ ] Finding the tangent direction at the collision
- [ ] Reducing velocity along that tangent using friction coefficients
- [ ] Adds realism for sliding, rolling, and stopping.

### Add Trigger (non-solid) Colliders and Collision Events (Optional)

Add a flag to colliders that prevents physical resolution.
Then detect:

- [ ] Enter events (colliding now but not last frame)
- [ ] Stay events
- [ ] Exit events

Physics Systems in the Correct Order

- Integration system (apply forces + move)
- Broad-phase collision detection
- Narrow-phase collision detection
- Collision resolution system
- Trigger/collision event system

### Add Debug Visualization Tools

To understand and tune your physics:

- Draw collider shapes
- Draw AABBs
- Draw contact normals
- Draw penetration depths
- Show spatial partitions (if using hashing or quadtrees)

