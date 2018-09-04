#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head node;
};

static LIST_HEAD(identity_list);

int identity_create(char *name, int id)
{
	struct identity *tmp;

	if (name == NULL)
		return -EINVAL;
	tmp = kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!tmp)
		return -ENOMEM;

	strncpy(tmp->name, name, 20);
	tmp->name[19] = '\0';
	tmp->id = id;
	tmp->busy = false;
	list_add(&tmp->node, &identity_list);

	return 0;
}

struct identity *identity_find(int id)
{
	struct identity *tmp;

	list_for_each_entry(tmp, &identity_list, node) {
		if (tmp->id == id)
			return tmp;
	}

	return NULL;
}

void identity_destroy(int id)
{
	struct identity *tmp;
	struct identity *tmp_safe;

	list_for_each_entry_safe(tmp, tmp_safe, &identity_list, node) {
		if (tmp->id == id) {
			list_del(&tmp->node);
			kfree(tmp);
		}
	}
}

static int __init hello_init(void)
{
	struct identity *temp;

	pr_debug("Hello World!\n");
	identity_create("Alice", 1);
	identity_create("Bob", 2);
	identity_create("Dave", 3);
	identity_create("Gena", 10);

	temp = identity_find(3);
	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
}

static void __exit hello_exit(void)
{
	pr_debug("Module unload\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My Task12");
MODULE_AUTHOR("Jamie Huang");
