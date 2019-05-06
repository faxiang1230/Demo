/*************************************************************************
    > File Name: sm4-skcipher-test.c
    > Author: wangjx
 ************************************************************************/
#include <linux/module.h>
#include <linux/crypto.h>
#include <crypto/algapi.h>
#include <linux/types.h>
#include <linux/init.h>                                                         
#include <linux/types.h>                                                        
#include <linux/errno.h>                                                        
#include <linux/crypto.h>                                                       
#include <asm/byteorder.h>                                                      
#include <asm/unaligned.h>                                                      
#include <crypto/internal/skcipher.h>                                           
#include <crypto/scatterwalk.h>

struct tcrypt_result {
    struct completion completion;
    int err;
};

/* tie all data structures together */
struct skcipher_def {
    struct scatterlist sg;
    struct crypto_skcipher *tfm;
    struct skcipher_request *req;
    struct tcrypt_result result;
};

/* Callback function */
static void test_skcipher_cb(struct crypto_async_request *req, int error)
{
    struct tcrypt_result *result = req->data;

    if (error == -EINPROGRESS)
        return;
    result->err = error;
    complete(&result->completion);
    pr_info("Encryption finished successfully\n");
}

/* Perform cipher operation */
static unsigned int test_skcipher_encdec(struct skcipher_def *sk,
                     int enc)
{
    int rc = 0;

    if (enc)
        rc = crypto_skcipher_encrypt(sk->req);
    else
        rc = crypto_skcipher_decrypt(sk->req);

    switch (rc) {
    case 0:
        break;
    case -EINPROGRESS:
    case -EBUSY:
        rc = wait_for_completion_interruptible(
            &sk->result.completion);
        if (!rc && !sk->result.err) {
            reinit_completion(&sk->result.completion);
            break;
        }
    default:
        pr_info("skcipher encrypt returned with %d result %d\n",
            rc, sk->result.err);
        break;
    }
    init_completion(&sk->result.completion);

    return rc;
}

/* Initialize and trigger cipher operation */
static int __init test_skcipher(void)
{
    struct skcipher_def sk;
    struct crypto_skcipher *skcipher = NULL;
    struct skcipher_request *req = NULL;
	int i = 0;

    unsigned char key[16] = {                                                   
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,                         
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,                         
    };
    unsigned char origin[32] = {                                                         
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,                         
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,                         
        0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,                         
        0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,                         
    }; 
    int ret = -EFAULT;

    skcipher = crypto_alloc_skcipher("ecb-sm4-generic", 0, 0);
    if (IS_ERR(skcipher)) {
        pr_info("could not allocate skcipher handle\n");
        return PTR_ERR(skcipher);
    }

    req = skcipher_request_alloc(skcipher, GFP_KERNEL);
    if (!req) {
        pr_info("could not allocate skcipher request\n");
        ret = -ENOMEM;
        goto out;
    }

    skcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
                      test_skcipher_cb,
                      &sk.result);

    if (crypto_skcipher_setkey(skcipher, key, 16)) {
        pr_info("key could not be set\n");
        ret = -EAGAIN;
        goto out;
    }

    sk.tfm = skcipher;
    sk.req = req;

    /* We encrypt one block */
    sg_init_one(&sk.sg, origin, 32);
    skcipher_request_set_crypt(req, &sk.sg, &sk.sg, 32, NULL);
	init_completion(&sk.result.completion);

	/* encrypt data */
	ret = test_skcipher_encdec(&sk, 1);
	if (ret)
		goto out;

	pr_info("Encryption triggered successfully\n");
	for (i = 0; i < 32; i++) {
		printk("%02x ", origin[i]);
	}
	printk("\n");

	/* decrypt data */
	ret = test_skcipher_encdec(&sk, 0);
	if (ret)
		goto out;

	pr_info("Decryption triggered successfully\n");
	for (i = 0; i < 32; i++) {
		printk("%02x ", origin[i]);
	}
	printk("\n");
out:
    if (skcipher)
        crypto_free_skcipher(skcipher);
    if (req)
        skcipher_request_free(req);
    return ret;
}
static void __exit test_skcipher_exit(void)
{
	return;
}
module_init(test_skcipher);
module_exit(test_skcipher_exit);
MODULE_LICENSE("GPL");
