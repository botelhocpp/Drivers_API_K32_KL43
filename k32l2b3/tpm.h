#ifndef TPM_H
#define TPM_H

#include <stdbool.h>

typedef enum {
	tpmCHANNEL_DISABLED = 0b0000,
	tpmCHANNEL_SOFTWARE_COMPARE = 0b0100,
	tpmCHANNEL_INPUT_CAPTURE_RE = 0b0001,
	tpmCHANNEL_INPUT_CAPTURE_FE = 0b0010,
	tpmCHANNEL_INPUT_CAPTURE_EE = 0b0011,
	tpmCHANNEL_OUTPUT_CAPTURE_TO = 0b0101,
	tpmCHANNEL_OUTPUT_CAPTURE_CO = 0b0110,
	tpmCHANNEL_OUTPUT_CAPTURE_SO = 0b0111,
	tpmCHANNEL_OUTPUT_CAPTURE_LM = 0b1110,
	tpmCHANNEL_OUTPUT_CAPTURE_HM = 0b1101,
	tpmCHANNEL_EDGE_ALIGNED_PWM_HT = 0b1010,
	tpmCHANNEL_EDGE_ALIGNED_PWM_LT = 0b1011,
	tpmCHANNEL_CENTER_ALIGNED_PWM_LT = 0b1010,
	tpmCHANNEL_CENTER_ALIGNED_PWM_HT = 0b1001
} tpm_channel_mode;

typedef enum {
	tpmPRESCALER_1,
	tpmPRESCALER_2,
	tpmPRESCALER_4,
	tpmPRESCALER_8,
	tpmPRESCALER_16,
	tpmPRESCALER_32,
	tpmPRESCALER_64,
	tpmPRESCALER_128
} tpm_prescaler;

typedef enum {
	tpmCHANNEL_0,
	tpmCHANNEL_1,
	tpmCHANNEL_2,
	tpmCHANNEL_3,
	tpmCHANNEL_4,
	tpmCHANNEL_5
} tpm_channel;

typedef enum {
	tpmMODULE_0,
	tpmMODULE_1,
	tpmMODULE_2
} tpm_module;

typedef enum {
	tpmCLOCK_MCGPCLK = 0b01,
	tpmCLOCK_OSCERCLK = 0b10,
	tpmCLOCK_MCGIRCLK = 0b11
} tpm_clock;

typedef struct {
	tpm_module module;
	tpm_channel channel;
	tpm_clock clk;
	uint16_t mod;
	uint16_t cnv;
	tpm_prescaler ps;
	tpm_channel_mode channel_mode;
} tpm_handler_t;

extern void tpmGetDefaultConfig(tpm_handler_t *tpm);

extern void tpmInitModule(tpm_handler_t *tpm);

extern void tpmSetModuleValue(tpm_handler_t *tpm, uint16_t module_value);

extern uint16_t tpmGetModuleValue(tpm_handler_t *tpm);

extern void tpmSetChannelValue(tpm_handler_t *tpm, uint16_t channel_value);

extern uint16_t tpmGetChannelValue(tpm_handler_t *tpm);

extern void tpmStartTimer(tpm_handler_t *tpm);

extern void tpmStopTimer(tpm_handler_t *tpm);

extern void tpmStartModuleTimer(tpm_module module);

extern void tpmStopModuleTimer(tpm_module module);

#endif
