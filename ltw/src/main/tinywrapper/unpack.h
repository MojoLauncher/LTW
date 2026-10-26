//
// Created by maks on 05.02.2026.
//

#ifndef GL4ES_WRAPPER_UNPACK_H
#define GL4ES_WRAPPER_UNPACK_H

void apply_unpack_state_to_hw(unpack_state_t *state);
void apply_default_state_to_hw();
bool is_unpack_compatible(GLenum internalformat, GLenum format, GLenum type);

#endif //GL4ES_WRAPPER_UNPACK_H
