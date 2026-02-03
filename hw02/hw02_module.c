#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

/* Module parameters */
#define MAX_STRING_LEN 15
static int idx = 0;
static char ch_val = 0;
static char my_str[MAX_STRING_LEN + 1];

/* Param idx */

static int parm_get_idx(char *buf, const struct kernel_param *kp) {
  return sprintf(buf, "%d", idx);
}

static int parm_set_idx(const char *val, const struct kernel_param *kp) {
  int ret;
  ret = kstrtoint(val, 10, &idx);
  if (ret)
    return ret;

  if (idx < 0 || idx >= MAX_STRING_LEN) {
    pr_err("Index out of bounds (0-%d)\n", MAX_STRING_LEN - 1);
    return -EINVAL;
  }

  pr_info("idx set to %d\n", idx);
  return 0;
}

static const struct kernel_param_ops parm_ops_idx = {
    .set = parm_set_idx,
    .get = parm_get_idx,
};
module_param_cb(idx, &parm_ops_idx, &idx, 0644);
MODULE_PARM_DESC(idx, "An index of character in my_str to set");

/* Param ch_val */

static int parm_get_ch_val(char *buf, const struct kernel_param *kp) {
  return sprintf(buf, "%c", ch_val);
}

static int parm_set_ch_val(const char *val, const struct kernel_param *kp) {
  if (strlen(val) != 1) {
    pr_err("Input must be a single character\n");
    return -EINVAL;
  }

  /* Check for ASCII control characters */
  if (val[0] <= 31 || val[0] == 127) {
    pr_err("Control characters are not allowed\n");
    return -EINVAL;
  }

  ch_val = val[0];
  my_str[idx] = ch_val;

  pr_info("ch_val set to %c\n", ch_val);
  return 0;
}

static const struct kernel_param_ops parm_ops_ch_val = {
    .set = parm_set_ch_val,
    .get = parm_get_ch_val,
};
module_param_cb(ch_val, &parm_ops_ch_val, &ch_val, 0644);
MODULE_PARM_DESC(ch_val, "A character value in my_str to set");

/* Param my_str */

static int parm_get_my_str(char *buf, const struct kernel_param *kp) {
  return sprintf(buf, "%s", my_str);
}

static int parm_set_my_str(const char *val, const struct kernel_param *kp) {
  return -EPERM; // Read-only
}

static const struct kernel_param_ops parm_ops_my_str = {
    .set = parm_set_my_str,
    .get = parm_get_my_str,
};
module_param_cb(my_str, &parm_ops_my_str, &my_str, 0444);
MODULE_PARM_DESC(my_str, "A string value");

/* Main code */

static int __init hw02_module_init(void) {
  pr_info("init\n");

  for (int i = 0; i <= MAX_STRING_LEN; i++) {
    my_str[i] = 0;
  }

  return 0;
}

static void __exit hw02_module_exit(void) { pr_info("exit\n"); }

module_init(hw02_module_init);
module_exit(hw02_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeny Vagin");
MODULE_DESCRIPTION("Otus Homework 02 Module");
