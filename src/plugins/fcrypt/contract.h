/**
 * @file
 *
 * @brief
 *
 * @copyright BSD License (see LICENSE.md or https://www.libelektra.org)
 */

keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME, KEY_VALUE, "fcrypt plugin waits for your orders", KEY_END),
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/exports", KEY_END),
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/exports/open", KEY_FUNC,
		ELEKTRA_PLUGIN_FUNCTION (ELEKTRA_PLUGIN_NAME, open), KEY_END),
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/exports/close", KEY_FUNC,
		ELEKTRA_PLUGIN_FUNCTION (ELEKTRA_PLUGIN_NAME, close), KEY_END),
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/exports/get", KEY_FUNC, ELEKTRA_PLUGIN_FUNCTION (ELEKTRA_PLUGIN_NAME, get),
		KEY_END),
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/exports/set", KEY_FUNC, ELEKTRA_PLUGIN_FUNCTION (ELEKTRA_PLUGIN_NAME, set),
		KEY_END),
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/exports/checkconf", KEY_FUNC,
		ELEKTRA_PLUGIN_FUNCTION (ELEKTRA_PLUGIN_NAME, checkconf), KEY_END),
#include ELEKTRA_README (fcrypt)
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/infos/version", KEY_VALUE, PLUGINVERSION, KEY_END),
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/constants/DEFAULT_TMPDIR", KEY_VALUE, ELEKTRA_FCRYPT_DEFAULT_TMPDIR,
		KEY_END),
	keyNew ("system/elektra/modules/" ELEKTRA_PLUGIN_NAME "/config/needs" ELEKTRA_FCRYPT_CONFIG_TMPDIR, KEY_VALUE,
		ELEKTRA_FCRYPT_DEFAULT_TMPDIR, KEY_END),
