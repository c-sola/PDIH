#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Clara");
MODULE_DESCRIPTION("Un módulo sencillo de prueba LKM");
MODULE_VERSION("0.1");

static char *name = "world";
module_param(name, charp, S_IRUGO);
MODULE_PARM_DESC(name, "El nombre que se mostrará en el log");

static int __init helloBBB_init(void) {
    printk(KERN_INFO "Hola %s desde el LKM!\n", name);
    return 0;
}

static void __exit helloBBB_exit(void) {
    printk(KERN_INFO "Adiós %s desde el LKM!\n", name);
}

module_init(helloBBB_init);
module_exit(helloBBB_exit);
