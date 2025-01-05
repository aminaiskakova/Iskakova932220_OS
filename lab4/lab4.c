#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/version.h>

MODULE_LICENSE("GPL");

#define PROC_FILENAME "tsulab"

static struct proc_dir_entry *proc_file;
// Функция вычисления количества месяцев с 04.07.1054 до 05.01.2025
static int calculate_months(void) {
    // Дата образования Крабовидной туманности: 04.07.1054
    int crab_year = 1054;
    int crab_month = 7;

    int now_year = 2025;
    int now_month = 1;

    int months = (now_year - crab_year) * 12 + (now_month - crab_month);

    return months;
}

static ssize_t tsulab_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char result[128];
    int len;

    int months_since_crab = calculate_months();

    len = snprintf(result, sizeof(result), "Крабовидная туманность существует: %d месяцев\n", months_since_crab);

    if (*ppos > 0 || count < len)
        return 0;

    if (copy_to_user(buf, result, len))
        return -EFAULT;

    *ppos = len;
    return len;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops tsulab_fops = {
    .proc_read = tsulab_read,
};
#else
static const struct proc_ops tsulab_fops = {
    .read = tsulab_read,
};
#endif

static int __init tsu_init(void) {
    pr_info("Welcome to the Tomsk State University\n");

    proc_file = proc_create(PROC_FILENAME, 0444, NULL, &tsulab_fops);
    if (!proc_file) {
        pr_err("Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }

    pr_info("/proc/%s created\n", PROC_FILENAME);
    return 0;
}

static void __exit tsu_exit(void) {
    if (proc_file) {
        proc_remove(proc_file);
        pr_info("/proc/%s removed\n", PROC_FILENAME);
    }

    pr_info("Tomsk State University forever!\n");
}

module_init(tsu_init);
module_exit(tsu_exit);
