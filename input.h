#pragma once
#include <stdbool.h>

typedef struct {
    bool left, right, up, down;
    bool action, jump, dash; // pour plus tard
} Input;

Input input_read(bool* running);
