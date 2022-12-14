/**
 * @file lcz_lwm2m_current.h
 * @brief
 *
 * Copyright (c) 2022 Laird Connectivity
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __LCZ_LWM2M_CURRENT_H__
#define __LCZ_LWM2M_CURRENT_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/* Global Function Prototypes                                                                     */
/**************************************************************************************************/
/**
 * @brief Set current using gateway object index to obtain base instance.
 * Object is created when needed and deleted if gateway object is deleted or instance is deleted.
 *
 * @param idx into gateway device table
 * @param offset of instance (obtained from advertisement event)
 * @param value in Amps
 * @return int negative error, 0 on success
 */
int lcz_lwm2m_managed_current_set(int idx, uint16_t offset, double value);

/**
 * @brief Set current. Requires instance to already be created.
 *
 * @param instance ID
 * @param value in Amps
 * @return int negative error, 0 on success
 */
int lcz_lwm2m_current_set(uint16_t instance, double value);

/**
 * @brief Create instance
 *
 * @param instance ID
 * @return int negative error, 0 on success
 */
int lcz_lwm2m_current_create(uint16_t instance);

#ifdef __cplusplus
}
#endif

#endif /* __LCZ_LWM2M_CURRENT_H__ */
