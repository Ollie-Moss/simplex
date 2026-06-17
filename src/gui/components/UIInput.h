#pragma once

struct UIInput
{
    std::string value;

    bool focused = false;
    bool readOnly = false;

    size_t cursor = 0;
    size_t selectionStart = 0;
    size_t selectionEnd = 0;

    bool operator==(const UIInput &) const
    {
        return true;
    }
};
