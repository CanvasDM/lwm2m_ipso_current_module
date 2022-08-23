/**
 * @file lcz_lwm2m_current.c
 * @brief
 *
 * Copyright (c) 2022 Laird Connectivity
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <logging/log.h>
LOG_MODULE_REGISTER(lwm2m_current, CONFIG_LCZ_LWM2M_CURRENT_LOG_LEVEL);

/**************************************************************************************************/
/* Includes                                                                                       */
/**************************************************************************************************/
#include <zephyr.h>
#include <init.h>
#include <lcz_lwm2m.h>

#include "lwm2m_resource_ids.h"
#include "lcz_snprintk.h"
#include "lcz_lwm2m.h"
#include "lcz_lwm2m_util.h"
#include "lcz_lwm2m_current.h"

/**************************************************************************************************/
/* Local Data Definitions                                                                         */
/**************************************************************************************************/
static struct lwm2m_obj_agent current_create_agent;

/**************************************************************************************************/
/* Local Function Prototypes                                                                      */
/**************************************************************************************************/
static int create_current_sensor(int idx, uint16_t type, uint16_t instance, void *context);

/**************************************************************************************************/
/* SYS INIT                                                                                       */
/**************************************************************************************************/
static int lcz_lwm2m_current_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	current_create_agent.type = IPSO_OBJECT_CURRENT_SENSOR_ID;
	current_create_agent.create = create_current_sensor;
	lcz_lwm2m_util_register_agent(&current_create_agent);

	return 0;
}

SYS_INIT(lcz_lwm2m_current_init, APPLICATION, LCZ_LWM2M_UTIL_USER_INIT_PRIORITY);

/**************************************************************************************************/
/* Global Function Definitions                                                                    */
/**************************************************************************************************/
#if defined(CONFIG_LCZ_LWM2M_UTIL_MANAGE_OBJ_INST)
int lcz_lwm2m_managed_current_set(int idx, uint16_t offset, double value)
{
	uint16_t type = IPSO_OBJECT_CURRENT_SENSOR_ID;
	int instance;
	int r;

	instance = lcz_lwm2m_util_manage_obj_instance(type, idx, offset);
	if (instance < 0) {
		LOG_ERR("Unable to find instance");
		return instance;
	}

	r = lcz_lwm2m_current_set(instance, value);
	lcz_lwm2m_util_manage_obj_deletion(r, type, idx, instance);
	return r;
}
#endif

int lcz_lwm2m_current_set(uint16_t instance, double value)
{
	char path[LWM2M_MAX_PATH_STR_LEN];

	LCZ_SNPRINTK(path, "%u/%u/%u", IPSO_OBJECT_CURRENT_SENSOR_ID, instance, SENSOR_VALUE_RID);
	return lwm2m_engine_set_float(path, &value);
}

int lcz_lwm2m_current_create(uint16_t instance)
{
	return lcz_lwm2m_util_create_obj_inst(IPSO_OBJECT_CURRENT_SENSOR_ID, instance);
}

/**************************************************************************************************/
/* Local Function Definitions                                                                     */
/**************************************************************************************************/
static int create_current_sensor(int idx, uint16_t type, uint16_t instance, void *context)
{
	ARG_UNUSED(idx);
	ARG_UNUSED(context);
	char units[] = "A";
	double min = 0.0;
	double max = 500.0;
	char path[LWM2M_MAX_PATH_STR_LEN];

	LCZ_SNPRINTK(path, "%u/%u/5701", type, instance);
	lwm2m_engine_set_string(path, units);

	/* 5603 and 5604 are the range of values supported by sensor. */
	LCZ_SNPRINTK(path, "%u/%u/5603", type, instance);
	lwm2m_engine_set_float(path, &min);

	LCZ_SNPRINTK(path, "%u/%u/5604", type, instance);
	lwm2m_engine_set_float(path, &max);

	return 0;
}
