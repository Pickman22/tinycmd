#include "unity.h"
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "utils.h"
#include "tinycmd.h"

stcode_t cmd_1(arg_t* args, void* usrargs) {
  (void)args;
  (void)usrargs;
  return ok_e;
}

extern stcode_t _get_cmddef(const char* cmdstr, const tinycmd_t * cmd_table, const cmddef_t** info);

/* extern stcode_t _get_param(const char* rawstr, char* param, uint8_t* done); */

extern stcode_t _parse_str(const char* rawstr, const tinycmd_t* table_sa, cmd_t* handle);

extern stcode_t _get_arg(const char* rawstr, const argdef_t* argdesc_a, arg_t* arg);

void test__get_param(void) {
   /*************************************************************************/
   /* TEST SETUP ************************************************************/
   /*************************************************************************/
   /* char str[] = "pwmfreq f2.33 m1"; */
   /* char str_a[][10] = { "pwmfreq", "f2.33", "m1" }; */
   /* char name[TINYCMD_NAME_MAX_SIZE]; */
   /* char* ofs = str; */
   /* stcode_t res; */
   /* uint8_t done = 0; */
   /* size_t i; */
   /* memset(name, 0, sizeof(name)); */

   /*************************************************************************/
   /* TEST ARGUMENT VALIDATION **********************************************/
   /*************************************************************************/
   /* res = _get_param(NULL, name, &done); */
   /* TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)res); */

   /* res = _get_param(str, NULL, &done); */
   /* TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)res); */

   /* res = _get_param(str, name, NULL); */
   /* TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)res); */

   /*************************************************************************/
   /* TEST BODY AND VALIDATION **********************************************/
   /*************************************************************************/
   /* for (i = 0; (!done) && (i < sizeof(str_a) / 10); ++i) { */
   /*    res = _get_param(ofs, name, &done); */
   /*    TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)res); */
   /*    TEST_ASSERT_TRUE(strcmp(str_a[i], name) == 0); */
   /*    ofs += strlen(name) + 1; */
   /*    if (i == 2) { */
   /*       TEST_ASSERT_TRUE(done); */
   /*    } */
   /* } */
}

stcode_t dummy_handle(arg_t* args, void* usrargs) {
   (void)usrargs;
   (void)args;
   /* int8_t r = TINYCMD_ARG(args, 0, int8_t); */
   /* int16_t q = TINYCMD_ARG(args, 1, int16_t); */
   /* int32_t f = TINYCMD_ARG(args, 2, int32_t); */
   return ok_e;
}

void test__get_cmdinfo(void) {
   /*************************************************************************/
   /* TEST SETUP ************************************************************/
   /*************************************************************************/

   uint8_t i;

   const cmddef_t info_a[] = {

      /*********************************************************************/
      {
         /* Command Name. */
         "pwmfreq",

         /* Command Handle. */
         dummy_handle,

         /* Argument Description. */
         {
            {arg_u8_e, 'r'},
            {arg_i16_e, 'q'},
            {arg_i32_e, 'f'},
         },

         /* User argument. */
         NULL,
      },
      /*********************************************************************/
      {
         /* Command Name. */
         "pid",

         /* Command Handle. */
         dummy_handle,

         /* Argument Description. */
         {
            {arg_i32_e, 'p'},
            {arg_i32_e, 'i'},
            {arg_i32_e, 'd'},
         },

         /* User argument. */
         NULL,
      },
      /*********************************************************************/
      {
         /* Command Name. */
         "ctrlmode",

         /* Command Handle. */
         dummy_handle,

         /* Argument Description. */
         {
            {arg_u8_e, 'x'},
            {arg_i16_e, 'y'},
            {arg_i32_e, 'z'},
         },

         /* User argument. */
         NULL,
      },
   };

   tinycmd_t cmdtable = { info_a, sizeof(info_a) / sizeof(cmddef_t) };

   const cmddef_t* p_cmdinfo_s;
   char cmdname[] = "pwmfreq";
   stcode_t ret;

   const char cmdname_a[][TINYCMD_NAME_MAX_SIZE] = { "pwmfreq", "pid", "ctrlmode" };

   /*************************************************************************/
   /* TEST ARGUMENT VALIDATION **********************************************/
   /*************************************************************************/
   ret = _get_cmddef(NULL, NULL, NULL);
   TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)ret);

   ret = _get_cmddef("pwmfreq", NULL, NULL);
   TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)ret);

   cmdtable.size = 0;
   ret = _get_cmddef("pwmfreq", &cmdtable, &p_cmdinfo_s);
   TEST_ASSERT_EQUAL_INT32((int32_t)inv_size_e, (int32_t)ret);
   cmdtable.size = sizeof(info_a) / sizeof(cmddef_t);

   /*************************************************************************/
   /* TEST BODY AND VALIDATION **********************************************/
   /*************************************************************************/
   ret = _get_cmddef(cmdname, &cmdtable, &p_cmdinfo_s);
   TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)ret);
   TEST_ASSERT_TRUE(cmdtable.cmdtab == p_cmdinfo_s);

   for (i = 0; i < sizeof(info_a) / sizeof(cmddef_t); i++) {
      ret = _get_cmddef(cmdname_a[i], &cmdtable, &p_cmdinfo_s);
      TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)ret);
      TEST_ASSERT_TRUE(&cmdtable.cmdtab[i] == p_cmdinfo_s);
   }

   // If command is non-existant, p_cmdinfo_s pointer is returned as NULL.
   p_cmdinfo_s = &info_a[0];
   ret = _get_cmddef("dummy", &cmdtable, &p_cmdinfo_s);
   TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)ret);
   TEST_ASSERT_TRUE(p_cmdinfo_s == NULL);
}

void test__parse_string(void) {
   /*************************************************************************/
   /* TEST SETUP ************************************************************/
   /*************************************************************************/

   const cmddef_t info_a[] = {

      /*********************************************************************/
      {
         /* Command Name. */
         "pwmfreq",

         /* Command Handle. */
         dummy_handle,

         /* Argument Description. */
         {
            {arg_u8_e, 'r'},
            {arg_i16_e, 'q'},
            {arg_i32_e, 'f'},
         },
         /* User argument. */
         NULL,
      },
      /*********************************************************************/
      {
         /* Command Name. */
         "pid",

         /* Command Handle. */
         dummy_handle,

         /* Argument Description. */
         {
            {arg_i32_e, 'p'},
            {arg_i32_e, 'i'},
            {arg_i32_e, 'd'},
         },
         /* User argument. */
         NULL,
      },
      /*********************************************************************/
      {
         /* Command Name. */
         "ctrlmode",

         /* Command Handle. */
         dummy_handle,

         /* Argument Description. */
         {
            {arg_u8_e, 'x'},
            {arg_i16_e, 'y'},
            {arg_i32_e, 'z'},
         },
         /* User argument. */
         NULL,
      },
   };

   stcode_t ret;
   char rawstr[TINYCMD_RAW_STR_MAX_SIZE] = "pwmfreq f233 r10 q-40";

   tinycmd_t table_sa;
   cmd_t handle = { NULL, {{0}}, NULL };
   table_sa.cmdtab = &info_a[0];
   table_sa.size = 0;
   ret = null_ptr_e;
   /*************************************************************************/
   /* TEST ARGUMENT VALIDATION **********************************************/
   /*************************************************************************/
   ret = _parse_str(NULL, NULL, NULL);
   TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)ret);

   ret = _parse_str(rawstr, NULL, NULL);
   TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)ret);

   ret = _parse_str(rawstr, &table_sa, NULL);
   TEST_ASSERT_EQUAL_INT32((int32_t)inv_size_e, (int32_t)ret);

   /*************************************************************************/
   /* TEST BODY AND VALIDATION **********************************************/
   /*************************************************************************/
   table_sa.size = sizeof(info_a) / sizeof(cmddef_t);
   ret = _parse_str(rawstr, &table_sa, &handle);
   TEST_ASSERT_TRUE(handle.callback == dummy_handle);
   TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)ret);
}

void test__get_arg(void) {
   /*************************************************************************/
   /* TEST SETUP ************************************************************/
   /*************************************************************************/

   uint8_t i;

   const argdef_t argdesc_a[] = {
      {arg_u8_e, 'a'},
      {arg_i8_e, 'b'},
      {arg_u16_e, 'c'},
      {arg_i16_e, 'd'},
      {arg_u32_e, 'e'},
      {arg_i32_e, 'f'},
   };

   const char argname_a[][TINYCMD_RAW_STR_MAX_SIZE] = {
      "a20",
      "b-32",
      "c65000",
      "d-32000",
      "e70000",
      "f-70000"
   };

   stcode_t ret;
   /* Done by Init function. */
   arg_t arg;
   int32_t values[] = { 20, -32, 65000, -32000, 70000, -70000 };

   uint32_t buf;

   memset(arg.data, 0, TINYCMD_ARG_BYTES_MAX_SIZE);
   /*************************************************************************/
   /* TEST ARGUMENT VALIDATION **********************************************/
   /*************************************************************************/

   /*************************************************************************/
   /* TEST BODY AND VALIDATION **********************************************/
   /*************************************************************************/
   ret = ok_e;
   for (i = 0; i < sizeof(argdesc_a) / sizeof(argdef_t); i++) {
      ret = _get_arg(argname_a[i], &argdesc_a[i], &arg);
      TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)ret);
      utils_from_bytes((void*)&arg.data[0], ((size_t)TINYCMD_ARG_BYTES_MAX_SIZE), (void*)&buf, sizeof(buf));
      switch (argdesc_a[i].type) {
      case arg_u8_e:
         TEST_ASSERT_TRUE(!memcmp((void*)&buf, (void*)&values[i], sizeof(uint8_t)));
         break;
      case arg_i8_e:
         TEST_ASSERT_TRUE(!memcmp((void*)&buf, (void*)&values[i], sizeof(int8_t)));
         break;
      case arg_u16_e:
         TEST_ASSERT_TRUE(!memcmp((void*)&buf, (void*)&values[i], sizeof(uint16_t)));
         break;
      case arg_i16_e:
         TEST_ASSERT_TRUE(!memcmp((void*)&buf, (void*)&values[i], sizeof(int16_t)));
         break;
      case arg_u32_e:
         TEST_ASSERT_TRUE(!memcmp((void*)&buf, (void*)&values[i], sizeof(uint32_t)));
         break;
      case arg_i32_e:
         TEST_ASSERT_TRUE(!memcmp((void*)&buf, (void*)&values[i], sizeof(int32_t)));
         break;
      default:
         break;

      }
   }
}

struct MyAppData {
   uint8_t a;
   int16_t b;
   int32_t c;
};
struct MyAppData appdata = { 1, 2, 3 };

stcode_t pwmfreq_handle(arg_t* args, void* usrargs) {
   uint8_t r = TINYCMD_ARG(args, 0, uint8_t);
   int16_t q = TINYCMD_ARG(args, 1, int16_t);
   int32_t f = TINYCMD_ARG(args, 2, int32_t);

   TEST_ASSERT_TRUE(f == 233);
   TEST_ASSERT_TRUE(TINYCMD_ARG_IS_VALID(args, 2));
   TEST_ASSERT_TRUE(r == 10);
   TEST_ASSERT_TRUE(TINYCMD_ARG_IS_VALID(args, 0));
   TEST_ASSERT_TRUE(q == -40);
   TEST_ASSERT_TRUE(TINYCMD_ARG_IS_VALID(args, 1));
   TEST_ASSERT_TRUE(usrargs == &appdata);

   return ok_e;
}

stcode_t pid_handle(arg_t* args, void* usrargs) {
   (void)args;
   (void)usrargs;
   return generic_e;
}

stcode_t ctrlmode_handle(arg_t* args, void* usrargs) {
   (void)usrargs;
   arg_t _args[TINYCMD_ARG_MAX_SIZE];
   memset(_args, 0, sizeof(arg_t) * (TINYCMD_ARG_MAX_SIZE));
   TEST_ASSERT_TRUE(!memcmp(_args, args, sizeof(arg_t) * (TINYCMD_ARG_MAX_SIZE)));
   return ok_e;
}

stcode_t set_mtr_vin(arg_t* args, void* usrargs) {
  return ok_e;
}

void test_cmd(void) {
   /*************************************************************************/
   /* TEST SETUP ************************************************************/
   /*************************************************************************/


   cmddef_t info_a[] = {

      /*********************************************************************/
      {
         /* Command Name. */
         "pwmfreq",

         /* Command Handle. */
         pwmfreq_handle,

         /* Argument Description. */
         {
            {arg_u8_e, 'r'},
            {arg_i16_e, 'q'},
            {arg_i32_e, 'f'},
         },

         /* User Arguments */
         (void*)&appdata,
      },
      /*********************************************************************/
      {
         /* Command Name. */
         "pid",

         /* Command Handle. */
         pid_handle,

         /* Argument Description. */
         {
            {arg_i32_e, 'p'},
            {arg_i32_e, 'i'},
            {arg_i32_e, 'd'},

         },

         /* User Arguments */
         TINYCMD_ARG_USR_NONE,
      },
      /*********************************************************************/
      {
         /* Command Name. */
         "ctrlmode",

         /* Command Handle. */
         ctrlmode_handle,

         /* Arguments. */
         TINYCMD_ARG_NONE,

         /*User Arguments */
         TINYCMD_ARG_USR_NONE,
      },
      {
        "mtrvin",
        set_mtr_vin,
        {
          {arg_i32_e, TINYCMD_UNIQUE_ARG},
        },
        TINYCMD_ARG_USR_NONE,
      }
   };

   stcode_t ret;
   char rawstr[TINYCMD_RAW_STR_MAX_SIZE] = "pwmfreq r10 f233 q-40";
   char rawstr1[TINYCMD_RAW_STR_MAX_SIZE] = "ctrlmode";
   char rawstr_mtr_vin[TINYCMD_RAW_STR_MAX_SIZE] = "set_mtr_vin 10";
   /*************************************************************************/
   /* TEST ARGUMENT VALIDATION **********************************************/
   /*************************************************************************/
   ret = tinycmd_init(NULL, 0);
   TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)ret);
   ret = tinycmd_init(info_a, 0);
   TEST_ASSERT_EQUAL_INT32((int32_t)inv_size_e, (int32_t)ret);
   ret = tinycmd_init(info_a, sizeof(info_a) / sizeof(cmddef_t));
   TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)ret);
   /*************************************************************************/
   /* TEST BODY AND VALIDATION **********************************************/
   /*************************************************************************/
   ret = tinycmd_exec(0);
   TEST_ASSERT_EQUAL_INT32((int32_t)null_ptr_e, (int32_t)ret);
   ret = tinycmd_exec(rawstr);
   TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)ret);
   ret = tinycmd_exec(rawstr1);
   TEST_ASSERT_EQUAL_INT32((int32_t)ok_e, (int32_t)ret);
}
