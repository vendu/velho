#ifndef __VLD_ELF32_H__
#define __VLD_ELF32_H__

#include <stdint.h>

/* flags for different object file sections */
#define ELF32_BSS_FLAGS         (ELF32_SHF_ALLOC | ELF32_SHF_WRITE)
#define ELF32_COMMENT_FLAGS     ELF32_SHF_NONE
#define ELF32_DATA_FLAGS        (ELF32_SHF_ALLOC | ELF32_SHF_WRITE)
#define ELF32_DATA1_FLAGS       (ELF32_SHF_ALLOC | ELF32_SHF_WRITE)
#define ELF32_DEBUG_FLAGS       ELF32_SHF_NONE
#define ELF32_DYNAMIC_FLAGS     ELF32_SHF_ALLOC
#define ELF32_DYNSTR_FLAGS      ELF32_SHF_ALLOC
#define ELF32_DYNSYM_FLAGS      ELF32_SHF_ALLOC
#define ELF32_FINI_FLAGS        (ELF32_SHF_ALLOC | ELF32_SHF_EXECINSTR)
#define ELF32_GOT_FLAGS         ELF32_SHF_NONE
#define ELF32_HASH_FLAGS        ELF32_SHF_ALLOC
#define ELF32_INIT_FLAGS        (ELF32_SHF_ALLOC | ELF32_SHF_EXECINSTR)
#define ELF32_INTERP_FLAGS      ELF32_SHF_NONE
#define ELF32_LINE_FLAGS        ELF32_SHF_NONE
#define ELF32_NOTE_FLAGS        ELF32_SHF_NONE
#define ELF32_PLT_FLAGS         ELF32_SHF_NONE
#define ELF32_REL_FLAGS         ELF32_SHF_NONE
#define ELF32_RELA_FLAGS        ELF32_SHF_NONE
#define ELF32_RODATA_FLAGS      ELF32_SHF_ALLOC
#define ELF32_RODATA1_FLAGS     ELF32_SHF_ALLOC
#define ELF32_SHSTRTAB_FLAGS    ELF32_SHF_NONE
#define ELF32_STRTAB_FLAGS      ELF32_SHF_NONE
#define ELF32_SYMTAB_FLAGS      ELF32_SHF_NONE
#define ELF32_TEXT_FLAGS        (ELF32_SHF_ALLOC | ELF32_SHF_EXECINSTR)

typedef uint32_t elf32adr;
typedef uint32_t elf32half;
typedef uint32_t elf32ofs;
typedef int32_t  elf32sword;
typedef uint32_t elf32word;
typedef uint8_t  elf32uchar;

/* ident-member */
#define ELF32_MAGIC_0           0
#define ELF32_MAGIC_1           1
#define ELF32_MAGIC_2           2
#define ELF32_MAGIC_3           3
#define ELF32_CLASS             4
#define ELF32_DATA              5
#define ELF32_VERSION           6
#define ELF32_PAD               7
#define ELF32_IDENT_LEN         16
#define ELF32_MAGIC_0_VAL       0x7f
#define ELF32_MAGIC_1_VAL       'E'
#define ELF32_MAGIC_2_VAL       'L'
#define ELF32_MAGIC_3_VAL       'F'
#define ELF32_CLASS_32          1
#define ELF32_CLASS_64          2
#define ELF32_DATA_LSB          1
#define ELF32_DATA_MSB          2
#define ELF32_VERSION           ELF32_CURRENT

/* type-member */
#define ELF32_NONE              0
#define ELF32_REL               1
#define ELF32_EXEC              2
#define ELF32_DYN               3
#define ELF32_CORE              4
#define ELF32_LOPROC            0xff00
#define ELF32_HIPROC            0xffff

/* machine-member */
#define ELF32_V0                0xffffffff

/* version-member */
#define ELF32_CURRENT           1

/* object file sections */
#define ELF32_SHN_UNDEF         0
#define ELF32_SHN_LORESERVE     0xff00
#define ELF32_SHN_LOPROC        0xff00
#define ELF32_SHN_HIPROC        0xff1f
#define ELF32_SHN_ABS           0xfff1
#define ELF32_SHN_COMMON        0xfff2
#define ELF32_SHN_HIRESERVE     0xffff

struct elf32hdr {
    elf32uchar  ident[ELF32_IDENT_LEN]; // object file header
    elf32half   type;                   // object file type
    elf32half   machine;                // machine type
    elf32half   version;                // object file version
    elf32half   entry;                  // entry point address
    elf32half   phofs;                  // program header table file offset
    elf32half   shofs;                  // section header table file offset
    elf32half   flags;                  // processor-specific flags
    elf32half   ehsize;                 // ELF header size
    elf32half   phentsize;              // program hedaer table entry size
    elf32half   phnum;                  // # of entries in program header table
    elf32half   shentsize;              // section header size
    elf32half   shnum;                  // # of entries in section header
    elf32half   tshstrndx;              // section header string table index
};

/* type-member */
#define ELF32_SHT_NULL          0
#define ELF32_SHT_PROGBITS      1
#define ELF32_SHT_SYMTAB        2
#define ELF32_SHT_STRTAB        3
#define ELF32_SHT_RELA          4
#define ELF32_SHT_HASH          5
#define ELF32_SHT_DYNAMIC       6
#define ELF32_SHT_NOTE          7
#define ELF32_SHT_NOBITS        8
#define ELF32_SHT_REL           9
#define ELF32_SHT_SHLIB         10
#define ELF32_SHT_DYNSYM        11
#define ELF32_SHT_LOPROC        0x70000000
#define ELF32_SHT_HIPROC        0x7fffffff
#define ELF32_SHT_LOUSER        0x80000000
#define ELF32_SHT_HIUSER        0xffffffff

/* flags-member */
#define ELF32_SHF_NONE          0x00000000
#define ELF32_SHF_WRITE         0x01
#define ELF32_SHF_ALLOC         0x02
#define ELF32_SHF_EXECINSTR     0x04
#define ELF32_SHF_MASKPROC      0xf0000000

struct elf32sechdr {
    elf32word   name;                   // section header string table index
    elf32word   type;                   // section header type
    elf32word   flags;
    elf32word   adr;
    elf32ofs    offset;
    elf32word   size;
    elf32word   link;
    elf32word   info;
    elf32word   adralign;
    elf32word   entsize;
};

#endif /* __VLD_ELF32_H__ */

