/**
 * \file
 *
 * \brief A plugin that makes use of libaugeas to read and write configuration files
 *
 * \copyright BSD License (see doc/COPYING or http://www.libelektra.org)
 *
 */

#ifdef HAVE_KDBCONFIG_H
#include "kdbconfig.h"
#endif

#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <tests_plugin.h>

void test_hostLensRead(char *fileName)
{
	Key *parentKey = keyNew ("user/tests/augeas-hosts", KEY_VALUE,
			srcdir_file (fileName), KEY_END);
	KeySet *conf = ksNew (20,
			keyNew ("system/lens", KEY_VALUE, "Hosts.lns", KEY_END), KS_END);
	PLUGIN_OPEN("augeas");

	KeySet *ks = ksNew (0);

	succeed_if(plugin->kdbGet (plugin, ks, parentKey) >= 1,
			"call to kdbGet was not successful");
	succeed_if(output_error (parentKey), "error in kdbGet");
	succeed_if(output_warnings (parentKey), "warnings in kdbGet");

	Key *key = ksLookupByName (ks, "user/tests/augeas-hosts/1/ipaddr", 0);
	exit_if_fail(key, "ip address of localhost not found");
	succeed_if(strcmp ("127.0.0.1", keyValue (key)) == 0,
			"address of localhost not correct");

	key = ksLookupByName (ks, "user/tests/augeas-hosts/1/canonical", 0);
	exit_if_fail(key, "name of localhost not found");
	succeed_if(strcmp ("localhost", keyValue (key)) == 0,
			"name of localhost not correct");

	key = ksLookupByName (ks, "user/tests/augeas-hosts/2/ipaddr", 0);
	exit_if_fail(key, "ip address of host1 not found");
	succeed_if(strcmp ("192.168.0.1", keyValue (key)) == 0,
			"address of host1 not correct");

	key = ksLookupByName (ks, "user/tests/augeas-hosts/2/canonical", 0);
	exit_if_fail(key, "name of host1 not found");
	succeed_if(strcmp ("host1", keyValue (key)) == 0,
			"name of host1 not correct");

	key = ksLookupByName (ks, "user/tests/augeas-hosts/2/alias[1]", 0);
	exit_if_fail(key, "alias1 of host1 not found");
	succeed_if(strcmp ("alias1", keyValue (key)) == 0,
			"name of alias1 of host1 not correct");

	key = ksLookupByName (ks, "user/tests/augeas-hosts/2/alias[2]", 0);
	exit_if_fail(key, "alias2 of host1 not found");
	succeed_if(strcmp ("alias2", keyValue (key)) == 0,
			"name of alias2 of host1 not correct");

	PLUGIN_CLOSE();

	ksDel (ks);
	keyDel(parentKey);
}

void test_hostLensWrite(char *fileName)
{
	char * fileNameCompare = malloc (strlen (fileName) + 6);
	strcpy (fileNameCompare, fileName);
	strcat (fileNameCompare, ".comp");

	Key *parentKey = keyNew ("user/tests/augeas-hosts", KEY_VALUE,
			srcdir_file (fileNameCompare), KEY_END);
	KeySet *conf = ksNew (20,
			keyNew ("system/lens", KEY_VALUE, "Hosts.lns", KEY_END), KS_END);
	PLUGIN_OPEN("augeas");

	KeySet *ks = ksNew (30, keyNew ("user/tests/augeas-hosts/1"),
			keyNew ("user/tests/augeas-hosts/1/ipaddr", KEY_VALUE, "127.0.0.1",
					KEY_META, "order", "10", KEY_END),
			keyNew ("user/tests/augeas-hosts/1/canonical", KEY_VALUE,
					"localhost", KEY_META, "order", "20", KEY_END),
			keyNew ("user/tests/augeas-hosts/1/#comment", KEY_VALUE,
					"hostcomment", KEY_META, "order", "21", KEY_END),
			keyNew ("user/tests/augeas-hosts/#comment", KEY_VALUE,
					"linecomment", KEY_META, "order", "22", KEY_END),
			keyNew ("user/tests/augeas-hosts/2/ipaddr", KEY_VALUE,
					"192.168.0.1", KEY_META, "order", "30", KEY_END),
			keyNew ("user/tests/augeas-hosts/2/canonical", KEY_VALUE, "host1",
					KEY_META, "order", "40", KEY_END),
			keyNew ("user/tests/augeas-hosts/2/alias[1]", KEY_VALUE,
					"host1alias1", KEY_META, "order", "50", KEY_END),
			keyNew ("user/tests/augeas-hosts/2/alias[2]", KEY_VALUE,
					"host1alias2", KEY_META, "order", "60", KEY_END),
			keyNew ("user/tests/augeas-hosts/3/ipaddr", KEY_VALUE,
					"fd00::4711:4712:2::1", KEY_META, "order", "70", KEY_END),
			keyNew ("user/tests/augeas-hosts/3/canonical", KEY_VALUE, "host2",
					KEY_META, "order", "80", KEY_END),
			keyNew ("user/tests/augeas-hosts/3/alias[1]", KEY_VALUE,
					"host2alias1", KEY_META, "order", "90", KEY_END),
			keyNew ("user/tests/augeas-hosts/3/alias[2]", KEY_VALUE,
					"host2alias2", KEY_META, "order", "100", KEY_END), KS_END);

	ksAppendKey (ks, parentKey);

	succeed_if(plugin->kdbSet (plugin, ks, parentKey) == 1,
			"kdbSet was not successful");
	succeed_if(output_error (parentKey), "error in kdbSet");
	succeed_if(output_warnings (parentKey), "warnings in kdbSet");

	free (fileNameCompare);

	succeed_if(
			compare_line_files (srcdir_file (fileName), keyString (parentKey)),
			"files do not match as expected");

	ksDel (ks);

	PLUGIN_CLOSE ()
	;
}

void test_hostLensModify(char *fileName)
{
	Key *parentKey = keyNew ("user/tests/augeas-hosts", KEY_VALUE,
			srcdir_file (fileName), KEY_END);
	KeySet *conf = ksNew (20,
			keyNew ("system/lens", KEY_VALUE, "Hosts.lns", KEY_END), KS_END);
	PLUGIN_OPEN("augeas");

	KeySet *ks = ksNew (0);

	succeed_if(plugin->kdbGet (plugin, ks, parentKey) >= 1,
			"call to kdbGet was not successful");
	succeed_if(output_error (parentKey), "error in kdbGet");
	succeed_if(output_warnings (parentKey), "warnings in kdbGet");

	Key *key = ksLookupByName (ks, "user/tests/augeas-hosts/1/ipaddr", 0);
	exit_if_fail(key, "ip address of localhost not found");
	keySetString (key, "127.0.0.2");

	key = ksLookupByName (ks, "user/tests/augeas-hosts/3/ipaddr", 0);
	exit_if_fail(key, "ip address of host2 not found");
	keySetString (key, "fd00::4711:4712:2::2");

	key = ksLookupByName (ks, "user/tests/augeas-hosts/#comment", 0);
	exit_if_fail(key, "line comment not found");
	keySetString (key, "line comment modified");

	char * fileNameCompare = malloc (strlen (fileName) + 6);
	strcpy (fileNameCompare, fileName);
	strcat (fileNameCompare, ".comp");

	keySetString (parentKey, srcdir_file(fileNameCompare));

	succeed_if(plugin->kdbSet (plugin, ks, parentKey) == 1,
			"kdbSet was not successful");
	succeed_if(output_error (parentKey), "error in kdbSet");
	succeed_if(output_warnings (parentKey), "warnings in kdbSet");

	free (fileNameCompare);

	succeed_if(
			compare_line_files (srcdir_file (fileName), keyString (parentKey)),
			"files do not match as expected");

	PLUGIN_CLOSE ();

	ksDel (ks);
	keyDel (parentKey);
}

void test_order(char *fileName)
{
	Key * parentKey = keyNew ("user/tests/augeas-hosts", KEY_VALUE,
			srcdir_file (fileName), KEY_END);
	KeySet *conf = ksNew (20,
			keyNew ("system/lens", KEY_VALUE, "Hosts.lns", KEY_END), KS_END);
	PLUGIN_OPEN("augeas");

	KeySet *ks = ksNew (0);

	succeed_if(plugin->kdbGet (plugin, ks, parentKey) >= 1,
			"call to kdbGet was not successful");
	succeed_if(output_error (parentKey), "error in kdbGet");
	succeed_if(output_warnings (parentKey), "warnings in kdbGet");

	Key *key;
	size_t currentIndex = 0;
	size_t numKeys = ksGetSize (ks);
	long *usedOrders = malloc (numKeys * sizeof(long));

	exit_if_fail(usedOrders, "unable to allocate memory for order array");

	/* as 0 is a legit order we have to initialize the array manually */
	for (size_t index = 0; index < numKeys; index++)
	{
		usedOrders[index] = -1;
	}

	ksRewind (ks);
	while ((key = ksNext (ks)) != 0)
	{
		if (strcmp (keyName (key), keyName (parentKey)))
		{
			char errorMessage[150];
			const Key *orderKey = keyGetMeta (key, "order");

			snprintf (errorMessage, 150, "key %s has no order", keyName (key));

			succeed_if(orderKey, errorMessage);

			char *orderString = (char *) keyValue (orderKey);
			long order;
			char *end;
			order = strtol (orderString, &end, 10);
			snprintf (errorMessage, 150, "key %s has an unparseable order",
					keyName (key));

			succeed_if(*end == 0, errorMessage);

			snprintf (errorMessage, 150, "key %s has a negative order",
					keyName (key));
			succeed_if(order >= 0, errorMessage);

			snprintf (errorMessage, 150,
					"the order %ld exists more than once. Duplicate found in %s.",
					order, keyName (key));

			// TODO: this is in O(n^2) where n is the number of keys
			for (size_t i = 0; i < currentIndex; i++)
			{
				succeed_if(usedOrders[i] != order, errorMessage);
			}

			usedOrders[currentIndex] = order;
			++currentIndex;
		}
	}

	free (usedOrders);
	ksDel (ks);
	keyDel(parentKey);

	PLUGIN_CLOSE() ;
}

int main(int argc, char** argv)
{
	printf ("AUGEAS       TESTS\n");
	printf ("==================\n\n");

	init (argc, argv);

	test_hostLensRead ("testdata/hosts-read");
	test_hostLensWrite ("testdata/hosts-write");
	test_hostLensModify ("testdata/hosts-modify");
	test_order ("testdata/hosts-big");

	printf ("\ntest_hosts RESULTS: %d test(s) done. %d error(s).\n", nbTest,
			nbError);

	return nbError;
}

