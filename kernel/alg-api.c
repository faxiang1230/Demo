#include <linux/if_alg.h>
#include <linux/socket.h>
#include <linux/module.h>
#include <crypto/skcipher.h>
#include <linux/slab.h>
#include <linux/scatterlist.h>
/* Initialize and trigger cipher operation */
static int __init test_skcipher(void)
{
	int i = 0, ret = 0;
    struct crypto_cipher *cipher = NULL;
	unsigned char output[16] = {0}, output2[16] = {0};
    //unsigned char key[32];
    unsigned char key[16] = {                                              
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,                         
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,                         
    };
    char origin[16] = {                                                         
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,                         
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,                         
    }; 

    cipher = crypto_alloc_cipher("sm4", 0, 0);
    if (IS_ERR(cipher)) {
        pr_info("could not allocate cipher handle\n");
        return PTR_ERR(cipher);
    }
    ret = crypto_cipher_setkey(cipher, key, 16);
	if (ret < 0) {
		pr_err("could not seteky\n");
		goto out;
	}
	crypto_cipher_encrypt_one(cipher, output, origin);
	printk("encrypt:");
	for (i = 0; i < 16; i++)
	   printk("%02x ", output[i]);
	printk("\n");	
    pr_info("Encryption triggered successfully\n");

	crypto_cipher_decrypt_one(cipher, output2, output);
	printk("decrypt:");
	for (i = 0; i < 16; i++)
	   printk("%02x ", output2[i]);
	printk("\n");	

out:
    if (cipher)
        crypto_free_cipher(cipher);
    return ret;
}
static void __exit test_skcipher_exit(void)
{
	return;
}
MODULE_LICENSE("GPL");
module_init(test_skcipher);
module_exit(test_skcipher_exit);
