#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
#include <signal.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/mman.h>
#include "ptrace_interface.h"
char shdrstr[] = {
"\0"
".shstrtab\0"
".interp\0"
".note.ABI-tag\0"
".note.gnu.build-id\0"
".gnu.hash\0"
".dynsym\0"
".dynstr\0"
".gnu.version\0"
".gnu.version_r\0"
".rela.dyn\0"
".rela.plt\0"
".init\0"
".plt.got\0"
".text\0"
".fini\0"
".rodata\0"
".eh_frame_hdr\0"
".eh_frame\0"
".init_array\0"
".fini_array\0"
".data.rel.ro\0"
".dynamic\0"
".data\0"
".bss\0"
".gnu_debuglink\0"
};
void dump_ehdr(Elf64_Ehdr *ehdr)
{
	printf("Elf64_Ehdr info:\n\tentry:%lx \tphnum:%d \tpentry size:%d\n",
			ehdr->e_entry, ehdr->e_phnum, ehdr->e_phentsize);
}
/* scan signature
 * 0000000000001d30 <__ctype_toupper_loc@plt-0x10>:                                     
 * 1d30:   ff 35 1a 00 21 00       pushq  0x21001a(%rip)        # 211d50 <quoting_style_args@@Base+0x250>                                     
 * 1d36:   ff 25 1c 00 21 00       jmpq   *0x21001c(%rip)        # 211d58 <quoting_style_args@@Base+0x258>
 * 1d3c:   0f 1f 40 00             nopl   0x0(%rax)
*/
Elf64_Addr find_plt(unsigned char *buf, int text_offset, int text_size)
{
	int offset = 0;
	while ((offset + 0x11) < text_size) {
		if (buf[offset] == 0xff && buf[offset + 1] == 0x35 &&
				buf[offset + 6] == 0xff && buf[offset + 7] == 0x25 &&
				buf[offset + 0xc] == 0x0f && buf[offset + 0xd] == 0x1f && buf[offset + 0xe] == 0x40 && buf[offset + 0xf] == 0x0 &&
				buf[offset+0x10]==0xff && buf[offset+0x11]==0x25)
			break;
		offset++;
	}
	return offset;
}
int main(int argc, char* argv[]) {
	int fd;
	char *buf = NULL, *pmem = NULL, *p = NULL;
	Elf64_Ehdr ehdr, *ehdr_ptr;
	Elf64_Phdr *phdr;
	pid_t pid;
	long ret = 0, i = 0, j = 0;
	Elf64_Addr text_base, text_size, data_offset, data_size, data_vaddr, bss_len, dynvaddr, dynoffset, dynsize, base_addr;
	Elf64_Addr interp_vaddr, interp_off, interp_size;
	Elf64_Addr stringTable, plt_siz, got_off, got;
	Elf64_Addr *GLOBAL_OFFSET_TABLE;
	Elf64_Sym *symtab;

	if(argc < 2) {
		printf("Usage :%s <pid>\n", argv[0]);
		return -1;
	}

	fd = open("123", O_CREAT|O_RDWR|O_TRUNC);
	pid = strtol(argv[1], NULL, 10);
	base_addr = get_mem_base(pid);

	ret = ptrace_attach(pid);
	printf("attach pid:%d ret:%ld\n", pid, ret);
	printf("base addr is:%lx\n", base_addr);
	ret = ptrace_read(&ehdr, sizeof(ehdr), (void *)base_addr, pid);
	dump_ehdr(&ehdr);

	printf("ehdr ret:%ld\n", ret);
	pmem = malloc(ehdr.e_phentsize * ehdr.e_phnum);
	ret = ptrace_read(pmem, ehdr.e_phentsize * ehdr.e_phnum, (void *)(base_addr + ehdr.e_phoff), pid);
	printf("phdr ret:%ld\n", ret);

	phdr = (Elf64_Phdr *)pmem;

	for (i = 0; i < ehdr.e_phnum; i++) {
		//printf("phnum:%d\n", i);
		if (phdr[i].p_type == PT_LOAD && !phdr[i].p_offset) {
			text_base = phdr[i].p_offset;
			text_size = phdr[i].p_filesz;
		}
		if (phdr[i].p_type == PT_LOAD && phdr[i].p_offset) {
			data_offset = phdr[i].p_offset;
			data_size = phdr[i].p_filesz;
			data_vaddr = phdr[i].p_vaddr;
			bss_len = phdr[i].p_memsz - phdr[i].p_filesz;
		}
		if (phdr[i].p_type == PT_DYNAMIC) {
			dynvaddr = phdr[i].p_vaddr;
			dynoffset = phdr[i].p_offset;
			dynsize = phdr[i].p_filesz;
		}
		if (phdr[i].p_type == PT_INTERP) {
			interp_vaddr = phdr[i].p_vaddr;
			interp_off = phdr[i].p_offset;
			interp_size = phdr[i].p_filesz;
		}
	}

	printf("prepare text\n");
	buf = calloc(1, data_offset + data_size);
	ret = ptrace_read(buf, text_size, (void *)base_addr, pid);
	printf("text ret:0x%lx\n", ret);
	ret = ptrace_read(buf + data_offset, data_size, (void *)(base_addr + data_vaddr), pid);
	printf("data ret:0x%lx\n", ret);

	printf("dynamic offset:%lx\n", dynoffset);
	Elf64_Dyn *dyn = (Elf64_Dyn *)(buf + dynoffset);
	for (i = 0; dyn[i].d_tag != DT_NULL; i++) {
		switch(dyn[i].d_tag)
		{
			case DT_PLTGOT:	//.got section
				dyn[i].d_un.d_ptr -= base_addr;
				printf("Located PLT GOT Vaddr 0x%lx index:%ld\n", got = (Elf64_Addr)dyn[i].d_un.d_ptr, i);
				printf("Relevant GOT entries begin at 0x%lx\n", (Elf64_Addr)dyn[i].d_un.d_ptr + 24);
				got_off = dyn[i].d_un.d_ptr - data_vaddr;
				GLOBAL_OFFSET_TABLE = (Elf64_Addr *)(buf + data_offset + got_off);
				printf("data offset:%lx got_off:%lx data_vaddr:0x%lx\n", data_offset, got_off, data_vaddr);
				GLOBAL_OFFSET_TABLE[0] = dynvaddr;
				GLOBAL_OFFSET_TABLE[1] = 0;
				GLOBAL_OFFSET_TABLE[2] = 0;
				GLOBAL_OFFSET_TABLE += 3;
				break;
			case DT_PLTRELSZ:
				plt_siz = dyn[i].d_un.d_val / sizeof(Elf64_Rela);
				break;
#if 0
			case DT_STRTAB:
				dyn[i].d_un.d_ptr -= base_addr;
				dynstr = (char *)dyn[i].d_un.d_ptr;
				break;
			case DT_SYMTAB:
				dyn[i].d_un.d_ptr -= base_addr;
				dynsym = (Elf64_Sym *)dyn[i].d_un.d_ptr;
				break;
#else
			case DT_STRTAB:
			case DT_SYMTAB:
#endif
			case DT_JMPREL:
			case DT_REL:
			case DT_VERSYM:
			case DT_RELA:
			case DT_GNU_HASH:
				printf("[+] Rebase dynamic entry %ld from %lx", dyn[i].d_tag, dyn[i].d_un.d_ptr);
				dyn[i].d_un.d_ptr -= base_addr;
				printf(" to %lx\n", dyn[i].d_un.d_ptr);
			case DT_DEBUG:
				dyn[i].d_un.d_ptr -= 0;
				break;
		}
	}

	printf("text_base:0x%lx text_size:0x%lx\n", text_base, text_size);
	Elf64_Addr PLT_VADDR = find_plt(buf, text_base, text_size);	
	printf("[+] Resolved PLT: 0x%lx\n", PLT_VADDR);
	PLT_VADDR + 0x10 + 0x6;
	for(i = 0; i < plt_siz; i ++) {
		//printf("Patch #%d - [0x%lx] changed to [0x%lx]\n", i, GLOBAL_OFFSET_TABLE[i], PLT_VADDR);
		GLOBAL_OFFSET_TABLE[i] = PLT_VADDR;
		PLT_VADDR += (i *0x10);
	}

	Elf64_Shdr *shdr = malloc(sizeof(Elf64_Shdr) * 10);
	//NULL section
	i = 0;
	memset(shdr, 0, sizeof(Elf64_Shdr));
	//.interp
	i++;
	shdr[i].sh_type = SHT_PROGBITS;
	shdr[i].sh_flags = SHF_ALLOC;
	shdr[i].sh_addralign = 1;
	shdr[i].sh_entsize = 0;
	shdr[i].sh_info = 0;
	shdr[i].sh_link = 0;
	shdr[i].sh_size = interp_size;
	shdr[i].sh_offset = interp_off;
	shdr[i].sh_addr = interp_off;
	for (j = 0, p = shdrstr;; j++)
		if (p[j] == '.' && p[j + 1] == 'i' && p[j + 2] == 'n' && p[j + 3] == 't' && p[j + 4] == 'e') {
			shdr[i].sh_name = j;break;
		}
	//.text
	i++ ;
	shdr[i].sh_type = SHT_PROGBITS;
	shdr[i].sh_flags = SHF_ALLOC|SHF_EXECINSTR;
	shdr[i].sh_addralign = 0xf;
	shdr[i].sh_entsize = 0;
	shdr[i].sh_info = 0;
	shdr[i].sh_link = 0;
	shdr[i].sh_offset = sizeof(ehdr)+ehdr.e_phnum*ehdr.e_phentsize;
	shdr[i].sh_size = text_size - shdr[i].sh_offset;
	shdr[i].sh_addr = shdr[i].sh_offset;
	for (j = 0, p = shdrstr;; j++)
		if (p[j] == '.' && p[j + 1] == 't' && p[j + 2] == 'e' && p[j + 3] == 'x' && p[j + 4] == 't') {
			shdr[i].sh_name = j;break;
		}
	//.data	
	i++ ;
	shdr[i].sh_type = SHT_PROGBITS;
	shdr[i].sh_flags = SHF_ALLOC|SHF_WRITE;
	shdr[i].sh_addralign = 0x20;
	shdr[i].sh_entsize = 0;
	shdr[i].sh_info = 0;
	shdr[i].sh_link = 0;
	shdr[i].sh_size = data_size;
	shdr[i].sh_offset = data_offset;
	shdr[i].sh_addr = data_vaddr;
	for (j = 0, p = shdrstr;; j++)
		if (p[j] == '.' && p[j + 1] == 'd' && p[j + 2] == 'a' && p[j + 3] == 't' && p[j + 4] == 'a' && p[j + 5] == '\0') {
			shdr[i].sh_name = j;break;
		}
	//.bss
	i++ ;
	shdr[i].sh_type = SHT_NOBITS;
	shdr[i].sh_flags = SHF_ALLOC|SHF_WRITE;
	shdr[i].sh_addralign = 0x20;
	shdr[i].sh_entsize = 0;
	shdr[i].sh_info = 0;
	shdr[i].sh_link = 0;
	shdr[i].sh_size = bss_len;
	shdr[i].sh_offset = data_offset + data_size;
	shdr[i].sh_addr = data_vaddr + shdr[i].sh_offset;
	for (j = 0, p = shdrstr;; j++)
		if (p[j] == '.' && p[j + 1] == 'b' && p[j + 2] == 's' && p[j + 3] == 's' && p[j + 4] == '\0') {
			shdr[i].sh_name = j;break;
		}
#if 0
	//.dynsym
	i++ ;
	shdr[i].sh_type = SHT_DYNSYM;
	shdr[i].sh_flags = SHF_ALLOC;
	shdr[i].sh_addralign = 0x8;
	shdr[i].sh_entsize = 0;
	shdr[i].sh_info = 0;
	shdr[i].sh_link = 0;
	shdr[i].sh_offset = dynsym;
	shdr[i].sh_size = dynsize;
	shdr[i].sh_addr = dynsym;
	for (j = 0, p = shdrstr;; j++)
		if (p[j] == '.' && p[j + 1] == 'd' && p[j + 2] == 'y' && p[j + 3] == 'n' && p[j + 4] == 's') {
			shdr[i].sh_name = j;break;
		}
#endif
	//.dynamic
	i++ ;
	shdr[i].sh_type = SHT_DYNAMIC;
	shdr[i].sh_flags = SHF_ALLOC|SHF_WRITE;
	shdr[i].sh_addralign = 0x8;
	shdr[i].sh_entsize = 0;
	shdr[i].sh_info = 0;
	shdr[i].sh_link = 0;
	shdr[i].sh_offset = dynoffset;
	shdr[i].sh_size = dynsize;
	shdr[i].sh_addr = dynvaddr;
	for (j = 0, p = shdrstr;; j++)
		if (p[j] == '.' && p[j + 1] == 'd' && p[j + 2] == 'y' && p[j + 3] == 'n' && p[j + 4] == 'a') {
			shdr[i].sh_name = j;break;
		}

	//.shstrtab	
	i++;
	shdr[i].sh_type = SHT_STRTAB;
	shdr[i].sh_flags = 0;
	shdr[i].sh_addralign = 1;   
	shdr[i].sh_entsize = 0;
	shdr[i].sh_info = 0;
	shdr[i].sh_link = 0;
	shdr[i].sh_size = sizeof(shdrstr);
	shdr[i].sh_offset = data_offset + data_size;
	shdr[i].sh_addr = 0;
	for (j = 0, p = shdrstr;; j++)
		if (p[j] == '.' && p[j + 1] == 's' && p[j + 2] == 'h' && p[j + 3] == 's' && p[j + 4] == 't') {
			shdr[i].sh_name = j;break;
		}
	ehdr_ptr = (Elf64_Ehdr *)buf;
	ehdr_ptr->e_shnum = i+1;
	ehdr_ptr->e_shstrndx = i;
	ehdr_ptr->e_shoff = data_offset+data_size+sizeof(shdrstr);

	write(fd, buf, data_offset + data_size);
	//printf("shstrtab offset:%x\n", lseek(fd, 0, SEEK_CUR));
	write(fd, shdrstr, sizeof(shdrstr));
	//printf("shstrtab offset:%x\n", lseek(fd, 0, SEEK_CUR));
	write(fd, shdr, sizeof(Elf64_Shdr)*(i+1));

	close(fd);
}
