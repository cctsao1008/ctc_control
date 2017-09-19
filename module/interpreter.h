/**
* @file interpreter.h
*
* interpreter
*
*   interpreter.
*
* @author Ricardo <tsao.ricardo@iac.com.tw>
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int rsh_interpreter_main(int argc, char *argv[]);
uint8_t pyc_command(const char *cmd);

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
