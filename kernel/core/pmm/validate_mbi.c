#include "common/types.h"   
#include "kernel/pmm/parse_mbi.h"
#include <kernel/pmm/validate_mbi.h>


static mbi_validation validate_mbi_header(multiboot_info_t *mbi) {
if (mbi-> size < sizeof(multiboot_info_t)) {
    return MBI_ERR_HEADER_SIZE;
} else {
    return MBI_VALID;
}
}

static mbi_validation validate_tag_bounds(const uint8_t *cursor, const uint8_t *mbi_end) {
if (cursor + sizeof(multiboot_tag_t) > mbi_end){
    return MBI_ERR_TAG_BOUNDS;
} else {
    return MBI_VALID;
}
}


static mbi_validation validate_tag(const multiboot_tag_t *tag, const uint8_t *cursor, const uint8_t *mbi_end, kbool_t *mmap_tag_exists) {
    if (tag->size < sizeof(multiboot_tag_t)){
        return MBI_ERR_TAG_SIZE;
    }
    if (cursor + tag->size > mbi_end) {
        return MBI_ERR_TAG_SUM_SIZE;
    }
    if (tag->type == MULTIBOOT_TAG_TYPE_MMAP ) {
        *mmap_tag_exists = true;
    }
    return MBI_VALID;
}


static mbi_validation validate_mmap_tag(const multiboot_tag_t* tag, const multiboot_tag_mmap_t *mmap, const uint8_t *cursor, const uint8_t *mmap_end) {
    if (mmap->size < sizeof(multiboot_tag_mmap_t)) {
        return MBI_ERR_MMAP_SIZE;
    }
    if (mmap_end > cursor + tag->size) {
        return MBI_ERR_MMAP_END;
    }
    if (mmap->entry_size < sizeof(multiboot_mmap_entry_t)) {
        return MBI_ERR_MMAP_ENTRY_SIZE;
    }
    return MBI_VALID;
}

mbi_validation validate_mbi(uint32_t mbi) {
    if (mbi == 0) {
        return MBI_ERR_NULL;
    }
    kbool_t mmap_tag_exists = false;
    multiboot_info_t *multiboot_i = (multiboot_info_t *)mbi;
    mbi_validation header_result = validate_mbi_header(multiboot_i);
    if (header_result != MBI_VALID) {
        return header_result;
    }
    uint8_t *cursor = (uint8_t *)(multiboot_i + 1);
    uint8_t *mbi_end = (uint8_t *)multiboot_i + multiboot_i->size;
    while (validate_tag_bounds(cursor, mbi_end) == MBI_VALID) {
         multiboot_tag_t *tag = (multiboot_tag_t *)cursor;
         if (tag->type == MULTIBOOT_TAG_TYPE_END) break;
         mbi_validation tag_result = validate_tag(tag, cursor, mbi_end, &mmap_tag_exists);
         if (tag_result != MBI_VALID) {
            return tag_result;
         }
         if (tag->type == MULTIBOOT_TAG_TYPE_MMAP) {
        multiboot_tag_mmap_t *mmap = (multiboot_tag_mmap_t *)tag;
        uint8_t *mmap_end = (uint8_t *)mmap + mmap->size;
        mbi_validation mmap_result = validate_mmap_tag(tag, mmap, cursor, mmap_end);
        if (mmap_result != MBI_VALID) {
            return mmap_result;
        }
        }
         cursor += (tag->size + MULTIBOOT_TAG_ALIGN - 1) & ~(MULTIBOOT_TAG_ALIGN - 1);
    }
    if (mmap_tag_exists != true) return MBI_ERR_NO_TAG_TYPE_MMAP;
    return MBI_VALID;
}