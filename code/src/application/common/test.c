/*
 * test.c
 *
 *  Created on: Oct 25, 2012
 *      Author: lutz
 */


#include <flawless/init/systemInitializer.h>
#include <flawless/core/msg_msgPump.h>
#include <flawless/config/msgIDs.h>

static void test_init(void);
MODULE_INIT_FUNCTION(test,NEVER, test_init);
static void test_init(void)
{
}

