#ifndef _SM_OPTIONS_H_
#define _SM_OPTIONS_H_

/**
 * @defgroup SMOptions Command-line Options
 * @{
 */

extern int simulatorUsesCPU; /**< Whether to restrict simulation to the CPU */

void SMOptionsParse(int argc, char * const * argv);

/** @} */

#endif
