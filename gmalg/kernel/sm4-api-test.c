#include <linux/if_alg.h>
#include <linux/socket.h>
#include <linux/module.h>
#include <crypto/skcipher.h>
#include <linux/slab.h>
#include <linux/scatterlist.h>
/* Initialize and trigger cipher operation */
static int __init test_cipher(void)
{
	int i = 0, ret = 0;
    struct crypto_blkcipher *cipher = NULL;
    struct blkcipher_desc desc = {
        .flags = CRYPTO_TFM_REQ_MAY_SLEEP,
    };
    struct scatterlist sg_out;
    struct scatterlist sg_in;
	unsigned char output[16] = {0}, output2[16] = {0};

    unsigned char key[16] = {                                              
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,                         
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,                         
    };
    char origin[16] = {                                                         
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,                         
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,                         
    }; 


    cipher = crypto_alloc_blkcipher("ecb(sm4)", 0, CRYPTO_ALG_ASYNC);
    if (IS_ERR(cipher)) {
        pr_info("could not allocate cipher handle\n");
        return PTR_ERR(cipher);
    }
    desc.tfm = cipher;

    ret = crypto_blkcipher_setkey(cipher, key, 16);
	if (ret < 0) {
		pr_err("could not seteky\n");
		goto out;
	}

    sg_init_one(&sg_in, origin, 16);
    sg_init_one(&sg_out, output, 16);
    crypto_blkcipher_encrypt(&desc, &sg_out, &sg_in, 16);

	printk("encrypt:");
	for (i = 0; i < 16; i++)
	   printk("%02x ", output[i]);
	printk("\n");	
    pr_info("Encryption triggered successfully\n");

    sg_set_buf(&sg_in, output, 16);
    sg_set_buf(&sg_out, output2, 16);
    crypto_blkcipher_decrypt(&desc, &sg_out, &sg_in, 16);

	printk("decrypt:");
	for (i = 0; i < 16; i++)
	   printk("%02x ", output2[i]);
	printk("\n");	

out:
    if (cipher)
        crypto_free_blkcipher(cipher);
    return ret;
}
static void __exit test_cipher_exit(void)
{
	return;
}
MODULE_LICENSE("GPL");
module_init(test_cipher);
module_exit(test_cipher_exit);
