#ifndef EDITOR_H
#define EDITOR_H

#include "item.h"

class Editor {
public:
    enum class Mode : uint8_t {
        StartAddLink,
        EndAddLink
    };

public:
    Editor();
};

#endif // EDITOR_H
