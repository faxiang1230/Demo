#include <linux/i2c.h>
## UNFINISHED
typedef enum {
	CHARGING_BATTERY = 0x01,
	CHARGING_AC		 = 0x02,
	CHARGING_USB	 = 0x03,
	CHARGING_MAX	 = 0x04,
} CHARGING_STATUS;

typedef struct {
	struct i2c_client *client;
	unsigned int reg;
	CHARGING_STATUS charging;
} bq_fuel;

static int foo_probe(struct i2c_client *client, const struct i2c_device_id *) {
	struct device_node *bq_node;
	bq_node = of_node_get(client->dev.of_node);

	bq_fuel *bq = devm_kzalloc(client->dev, sizeof(bq_fuel), GFP_KERNEL);
	if(bq == NULL) {
		dev_err(client->dev, "bq memory alloc failed\n");
		goto failed;
	}
	i2c_set_clientdata(client, bq);

	return 0;
failed:
	return -1;
}
static int foo_remove(struct i2c_client *) {
	return 0;
}
static int foo_detect(struct i2c_client *, struct i2c_board_info *) {
	return 0;
}
static int foo_command(struct i2c_client *client, unsigned int cmd, void *arg) {
	return 0;
}
static void foo_shutdown(struct i2c_client *){
}
#ifdef CONFIG_PM_SLEEP
static int foo_suspend(struct device *dev) {
	return 0;
}
static int foo_resume(struct device *dev) {
	return 0;
}
#else
static int foo_suspend(struct device *dev) {return 0;}
static int foo_resume(struct device *dev) {return 0;}
#endif
static const struct dev_pm_ops foo_pm_ops = {
	.suspend_noirq = foo_suspend,
	.resume_noirq = foo_resume,
};
static struct i2c_device_id foo_idtable[] = {
	{ "foo", 0 },
	{ "ti,bq27531", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, foo_idtable);

static struct i2c_driver foo_driver = {
	.driver = {
		.name	= "foo",
		.pm	= &foo_pm_ops,	/* optional */
	},

	.id_table	= foo_idtable,
	.probe		= foo_probe,
	.remove		= foo_remove,
	/* if device autodetection is needed: */
	.class		= I2C_CLASS_SOMETHING,
	.detect		= foo_detect,
	.address_list	= normal_i2c,

	.shutdown	= foo_shutdown,	/* optional */
	.command	= foo_command,	/* optional, deprecated */
}
module_i2c_driver(foo_driver);

/* Substitute your own name and email address */
MODULE_AUTHOR("Jianxing.Wang <wangjianxing5210@163.com>");
MODULE_DESCRIPTION("Driver for Sample I2C device");
/* a few non-GPL license types are also allowed */
MODULE_LICENSE("GPL v2");
