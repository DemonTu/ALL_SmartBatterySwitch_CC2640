package ti.targets.omf.elf;

import java.io.RandomAccessFile;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;

/*
 *  ======== Dwarf32 ========
 */
public class Dwarf32
{
    /*
     *  ======== attributes ========
     */
    private static final int DW_AT_sibling = 0x01; /* reference */
    private static final int DW_AT_location = 0x02; /* block, loclistptr */
    private static final int DW_AT_name = 0x03; /* string */

    private static final int DW_AT_ordering = 0x09; /* constant */

    private static final int DW_AT_byte_size = 0x0b; /* block, constant, reference */
    private static final int DW_AT_bit_offset = 0x0c; /* block, constant, reference */
    private static final int DW_AT_bit_size = 0x0d; /* block, constant, reference */

    private static final int DW_AT_stmt_list = 0x10; /* lineptr */
    private static final int DW_AT_low_pc = 0x11; /* address */
    private static final int DW_AT_high_pc = 0x12; /* address */
    private static final int DW_AT_language = 0x13; /* constant */

    private static final int DW_AT_discr = 0x15; /* reference */
    private static final int DW_AT_discr_value = 0x16; /* constant */
    private static final int DW_AT_visibility = 0x17; /* constant */
    private static final int DW_AT_import = 0x18; /* reference */
    private static final int DW_AT_string_length = 0x19; /* block, loclistptr */
    private static final int DW_AT_common_reference = 0x1a; /* reference */
    private static final int DW_AT_comp_dir = 0x1b; /* string */
    private static final int DW_AT_const_value = 0x1c; /* block, constant, string */
    private static final int DW_AT_containing_type = 0x1d; /* reference */
    private static final int DW_AT_default_value = 0x1e; /* reference */

    private static final int DW_AT_inline = 0x20; /* constant */
    private static final int DW_AT_is_optional = 0x21; /* flag */
    private static final int DW_AT_lower_bound = 0x22; /* block, constant, reference */

    private static final int DW_AT_producer = 0x25; /* string */

    private static final int DW_AT_prototyped = 0x27; /* flag */

    private static final int DW_AT_return_addr = 0x2a; /* block, loclistptr */

    private static final int DW_AT_start_scope = 0x2c; /* constant */

    private static final int DW_AT_bit_stride = 0x2e; /* constant */
    private static final int DW_AT_upper_bound =  0x2f; /* block, constant, reference */

    private static final int DW_AT_abstract_origin = 0x31; /* reference */
    private static final int DW_AT_accessibility = 0x32; /* constant */
    private static final int DW_AT_address_class = 0x33; /* constant */
    private static final int DW_AT_artificial = 0x34; /* flag */
    private static final int DW_AT_base_types = 0x35; /* reference */
    private static final int DW_AT_calling_convention = 0x36; /* constant */
    private static final int DW_AT_count = 0x37; /* block, constant, reference */
    private static final int DW_AT_data_member_location = 0x38; /* block, constant, loclistptr */
    private static final int DW_AT_decl_column =  0x39; /* constant */
    private static final int DW_AT_decl_file = 0x3a; /* constant */
    private static final int DW_AT_decl_line = 0x3b; /* constant */
    private static final int DW_AT_declaration = 0x3c; /* flag */
    private static final int DW_AT_discr_list = 0x3d; /* block */
    private static final int DW_AT_encoding = 0x3e; /* constant */
    private static final int DW_AT_external = 0x3f; /* flag */
    private static final int DW_AT_frame_base = 0x40; /* block, loclistptr */
    private static final int DW_AT_friend = 0x41; /* reference */
    private static final int DW_AT_identifier_case = 0x42; /* constant */
    private static final int DW_AT_macro_info = 0x43; /* macptr */
    private static final int DW_AT_namelist_item = 0x44; /* block */
    private static final int DW_AT_priority = 0x45; /* reference */
    private static final int DW_AT_segment = 0x46; /* block, loclistptr */
    private static final int DW_AT_specification = 0x47; /* reference */
    private static final int DW_AT_static_link = 0x48; /* block, loclistptr */
    private static final int DW_AT_type = 0x49; /* reference */
    private static final int DW_AT_use_location = 0x4a; /* block, loclistptr */
    private static final int DW_AT_variable_parameter = 0x4b; /* flag */
    private static final int DW_AT_virtuality = 0x4c; /* constant */
    private static final int DW_AT_vtable_elem_location = 0x4d; /* block, loclistptr */
    private static final int DW_AT_allocated = 0x4e; /* block, constant, reference */
    private static final int DW_AT_associated = 0x4f ;/* block, constant, reference */
    private static final int DW_AT_data_location = 0x50; /* block */
    private static final int DW_AT_byte_stride = 0x51; /* block, constant, reference */
    private static final int DW_AT_entry_pc = 0x52; /* address */
    private static final int DW_AT_use_UTF8 = 0x53; /* flag */
    private static final int DW_AT_extension = 0x54; /* reference */
    private static final int DW_AT_ranges = 0x55; /* rangelistptr */
    private static final int DW_AT_trampoline = 0x56; /* address, flag, reference, string */
    private static final int DW_AT_call_column = 0x57; /* constant */
    private static final int DW_AT_call_file = 0x58; /* constant */
    private static final int DW_AT_call_line = 0x59; /* constant */
    private static final int DW_AT_description = 0x5a; /* string */
    private static final int DW_AT_binary_scale = 0x5b; /* constant */
    private static final int DW_AT_decimal_scale = 0x5c; /* constant */
    private static final int DW_AT_small = 0x5d; /* reference */
    private static final int DW_AT_decimal_sign = 0x5e; /* constant */
    private static final int DW_AT_digit_count = 0x5f; /* constant */
    private static final int DW_AT_picture_string = 0x60; /* string */
    private static final int DW_AT_mutable = 0x61; /* flag */
    private static final int DW_AT_threads_scaled = 0x62; /* flag */
    private static final int DW_AT_explicit = 0x63; /* flag */
    private static final int DW_AT_object_pointer = 0x64; /* reference */
    private static final int DW_AT_endianity = 0x65; /* constant */
    private static final int DW_AT_elemental = 0x66; /* flag */

    private static final int DW_AT_lo_user = 0x2000;
    private static final int DW_AT_hi_user = 0x3fff;

    /*
     *  ======== tags ========
     */
    private static final int DW_TAG_array_type = 0x01;
    private static final int DW_TAG_class_type = 0x02;
    private static final int DW_TAG_entry_point = 0x03;
    private static final int DW_TAG_enumeration_type = 0x04;
    private static final int DW_TAG_formal_parameter = 0x05;
    private static final int DW_TAG_imported_declaration = 0x08;
    private static final int DW_TAG_label = 0x0a;
    private static final int DW_TAG_lexical_block = 0x0b;
    private static final int DW_TAG_member = 0x0d;
    private static final int DW_TAG_pointer_type = 0x0f;
    private static final int DW_TAG_reference_type = 0x10;
    private static final int DW_TAG_compile_unit = 0x11;
    private static final int DW_TAG_string_type = 0x12;
    private static final int DW_TAG_structure_type = 0x13;
    private static final int DW_TAG_subroutine_type = 0x15;
    private static final int DW_TAG_typedef = 0x16;
    private static final int DW_TAG_union_type = 0x17;
    private static final int DW_TAG_unspecified_parameters = 0x18;
    private static final int DW_TAG_variant = 0x19;
    private static final int DW_TAG_common_block = 0x1a;
    private static final int DW_TAG_common_inclusion = 0x1b;
    private static final int DW_TAG_inheritance = 0x1c;
    private static final int DW_TAG_inlined_subroutine = 0x1d;
    private static final int DW_TAG_module = 0x1e;
    private static final int DW_TAG_ptr_to_member_type = 0x1f;
    private static final int DW_TAG_set_type = 0x20;
    private static final int DW_TAG_subrange_type = 0x21;
    private static final int DW_TAG_with_stmt = 0x22;
    private static final int DW_TAG_access_declaration = 0x23;
    private static final int DW_TAG_base_type = 0x24;
    private static final int DW_TAG_catch_block = 0x25;
    private static final int DW_TAG_const_type = 0x26;
    private static final int DW_TAG_constant = 0x27;
    private static final int DW_TAG_enumerator = 0x28;
    private static final int DW_TAG_file_type = 0x29;
    private static final int DW_TAG_friend = 0x2a;
    private static final int DW_TAG_namelist = 0x2b;
    private static final int DW_TAG_namelist_item = 0x2c;
    private static final int DW_TAG_packed_type = 0x2d;
    private static final int DW_TAG_subprogram = 0x2e;
    private static final int DW_TAG_template_type_parameter = 0x2f;
    private static final int DW_TAG_template_value_parameter = 0x30;
    private static final int DW_TAG_thrown_type = 0x31;
    private static final int DW_TAG_try_block = 0x32;
    private static final int DW_TAG_variant_part = 0x33;
    private static final int DW_TAG_variable = 0x34;
    private static final int DW_TAG_volatile_type = 0x35;
    private static final int DW_TAG_dwarf_procedure = 0x36;
    private static final int DW_TAG_restrict_type = 0x37;
    private static final int DW_TAG_interface_type = 0x38;
    private static final int DW_TAG_namespace = 0x39;
    private static final int DW_TAG_imported_module = 0x3a;
    private static final int DW_TAG_unspecified_type = 0x3b;
    private static final int DW_TAG_partial_unit = 0x3c;
    private static final int DW_TAG_imported_unit = 0x3d;
    private static final int DW_TAG_condition = 0x3f;
    private static final int DW_TAG_shared_type = 0x40;

    private static final int DW_TAG_lo_user = 0x4080;
    private static final int DW_TAG_hi_user = 0xffff;

   /*
    *  ======== forms ========
    */
    private static final int DW_FORM_addr = 0x01; /* address */
    private static final int DW_FORM_block2 = 0x03; /* block */
    private static final int DW_FORM_block4 = 0x04; /* block */
    private static final int DW_FORM_data2 = 0x05; /* constant */
    private static final int DW_FORM_data4 = 0x06; /* constant, lineptr, loclistptr, macptr, rangelistptr */
    private static final int DW_FORM_data8 = 0x07; /* constant, lineptr, loclistptr, macptr, rangelistptr */
    private static final int DW_FORM_string = 0x08; /* string */
    private static final int DW_FORM_block = 0x09; /* block */
    private static final int DW_FORM_block1 = 0x0a; /* block */
    private static final int DW_FORM_data1 = 0x0b; /* constant */
    private static final int DW_FORM_flag = 0x0c; /* flag */
    private static final int DW_FORM_sdata = 0x0d; /* constant */
    private static final int DW_FORM_strp = 0x0e; /* string */
    private static final int DW_FORM_udata = 0x0f; /* constant */
    private static final int DW_FORM_ref_addr = 0x10; /* reference */
    private static final int DW_FORM_ref1 = 0x11; /* reference */
    private static final int DW_FORM_ref2 = 0x12; /* reference */
    private static final int DW_FORM_ref4 = 0x13; /* reference */
    private static final int DW_FORM_ref8 = 0x14; /* reference */
    private static final int DW_FORM_ref_udata = 0x15; /* reference */
    private static final int DW_FORM_indirect = 0x16 ;/* see Section 7.5.  */

    /*
     *  ======== Form ========
     */
    private static class Form {
        public String name;
        public int size;
        
        public Form (String n, int sz) {
            this.name = n;
            this.size = sz;
        }
    }

    /*
     *  ======== forms ========
     *  Array of all DW_FORM* 
     */
    private static Form[] forms = {
        new Form("null", -1),
        new Form("DW_FORM_addr", -1),    /* 0x01,  address (size defined in CU header) */
        new Form("0x02", -1),            /* 0x02 */
        new Form("DW_FORM_block2", 2),   /* 0x03,  block */
        new Form("DW_FORM_block4", 4),   /* 0x04,  block */
        new Form("DW_FORM_data2", 2),    /* 0x05,  constant */
        new Form("DW_FORM_data4", 4),    /* 0x06,  constant, lineptr, loclistptr, macptr, rangelistptr */
        new Form("DW_FORM_data8", 8),    /* 0x07,  constant, lineptr, loclistptr, macptr, rangelistptr */
        new Form("DW_FORM_string", 0),   /* 0x08,  string ('\0' terminated chars) */
        new Form("DW_FORM_block", 0),    /* 0x09,  block (unsigned LEB128) */
        new Form("DW_FORM_block1", 1),   /* 0x0a,  block */
        new Form("DW_FORM_data1", 1),    /* 0x0b,  constant */
        new Form("DW_FORM_flag", 1),     /* 0x0c,  flag */
        new Form("DW_FORM_sdata", 0),    /* 0x0d,  constant (signed LEB128) */
        new Form("DW_FORM_strp", 4),     /* 0x0e,  string (8 for 64-bit dwarf) */
        new Form("DW_FORM_udata", 0),    /* 0x0f,  constant (unsigned LEB128) */
        new Form("DW_FORM_ref_addr", 4), /* 0x10,  reference (8 for 64-bit dwarf) */
        new Form("DW_FORM_ref1", 1),     /* 0x11,  reference */
        new Form("DW_FORM_ref2", 2),     /* 0x12,  reference */
        new Form("DW_FORM_ref4", 4),     /* 0x13,  reference */
        new Form("DW_FORM_ref8", 8),     /* 0x14,  reference */
        new Form("DW_FORM_ref_udata", 0),/* 0x15,  reference (unsigned LEB128) */
        new Form("DW_FORM_indirect", 0), /* 0x16,  see Section 7.5. (unsigned LEB128) */
    };
 
    /*
     *  ======== tagNames ========
     *  Array of all DW_TAG*
     */
    private static String[] tagNames = {
        "null",                         /* 0x00 */
        "DW_TAG_array_type",            /* 0x01 */
        "DW_TAG_class_type",            /* 0x02 */
        "DW_TAG_entry_point",           /* 0x03 */
        "DW_TAG_enumeration_type",      /* 0x04 */
        "DW_TAG_formal_parameter",      /* 0x05 */
        "0x06",                         /* 0x06 */
        "0x07",                         /* 0x07 */
        "DW_TAG_imported_declaration",  /* 0x08 */
        "0x09",                         /* 0x09 */
        "DW_TAG_label",                 /* 0x0a */
        "DW_TAG_lexical_block",         /* 0x0b */
        "0x0c",                         /* 0x0c */
        "DW_TAG_member",                /* 0x0d */
        "0x0e",                         /* 0x0e */
        "DW_TAG_pointer_type",          /* 0x0f */
        "DW_TAG_reference_type",        /* 0x10 */
        "DW_TAG_compile_unit",          /* 0x11 */
        "DW_TAG_string_type",           /* 0x12 */
        "DW_TAG_structure_type",        /* 0x13 */
        "0x14",                         /* 0x14 */
        "DW_TAG_subroutine_type",       /* 0x15 */
        "DW_TAG_typedef",               /* 0x16 */
        "DW_TAG_union_type",            /* 0x17 */
        "DW_TAG_unspecified_parameters",/* 0x18 */
        "DW_TAG_variant",               /* 0x19 */
        "DW_TAG_common_block",          /* 0x1a */
        "DW_TAG_common_inclusion",      /* 0x1b */
        "DW_TAG_inheritance",           /* 0x1c */
        "DW_TAG_inlined_subroutine",    /* 0x1d */
        "DW_TAG_module",                /* 0x1e */
        "DW_TAG_ptr_to_member_type",    /* 0x1f */
        "DW_TAG_set_type",              /* 0x20 */
        "DW_TAG_subrange_type",         /* 0x21 */
        "DW_TAG_with_stmt",             /* 0x22 */
        "DW_TAG_access_declaration",    /* 0x23 */
        "DW_TAG_base_type",             /* 0x24 */
        "DW_TAG_catch_block",           /* 0x25 */
        "DW_TAG_const_type",            /* 0x26 */
        "DW_TAG_constant",              /* 0x27 */
        "DW_TAG_enumerator",            /* 0x28 */
        "DW_TAG_file_type",             /* 0x29 */
        "DW_TAG_friend",                /* 0x2a */
        "DW_TAG_namelist",              /* 0x2b */
        "DW_TAG_namelist_item",         /* 0x2c */
        "DW_TAG_packed_type",           /* 0x2d */
        "DW_TAG_subprogram",            /* 0x2e */
        "DW_TAG_template_type_parameter",  /* 0x2f */
        "DW_TAG_template_value_parameter", /* 0x30 */
        "DW_TAG_thrown_type",           /* 0x31 */
        "DW_TAG_try_block",             /* 0x32 */
        "DW_TAG_variant_part",          /* 0x33 */
        "DW_TAG_variable",              /* 0x34 */
        "DW_TAG_volatile_type",         /* 0x35 */
        "DW_TAG_dwarf_procedure",       /* 0x36 */
        "DW_TAG_restrict_type",         /* 0x37 */
        "DW_TAG_interface_type",        /* 0x38 */
        "DW_TAG_namespace",             /* 0x39 */
        "DW_TAG_imported_module",       /* 0x3a */
        "DW_TAG_unspecified_type",      /* 0x3b */
        "DW_TAG_partial_unit",          /* 0x3c */
        "DW_TAG_imported_unit",         /* 0x3d */
        "0x3e",                         /* 0x3e */
        "DW_TAG_condition",             /* 0x3f */
        "DW_TAG_shared_type",           /* 0x40 */
        "DW_TAG_type_unit",             /* 0x41 (Dwarf4) */
        "DW_TAG_rvalue_reference_type", /* 0x42 (Dwarf4) */
        "DW_TAG_template_alias",        /* 0x43 (Dwarf4) */
    };


    /*
     *  ======== attrNames ========
     *  Array of all DW_AT*
     */
    private static String[] attrNames = {
        "null",                     /* 0x00 */
        "DW_AT_sibling",            /* 0x01 (CU) reference */
        "DW_AT_location",           /* 0x02 */
        "DW_AT_name",               /* 0x03 (CU) string */
        "???",
        "???",
        "???",
        "???",
        "???",
        "DW_AT_ordering",           /* 0x09 */
        "???",
        "DW_AT_byte_size",          /* 0x0b */
        "DW_AT_bit_offset",         /* 0x0c */
        "DW_AT_bit_size",           /* 0x0d */
        "???",
        "???",
        "DW_AT_stmt_list",          /* 0x10 (CU) lineptr */
        "DW_AT_low_pc",             /* 0x11 (CU) address */
        "DW_AT_high_pc",            /* 0x12 (CU) address */
        "DW_AT_language",           /* 0x13 (CU) constant (LEB128) */
        "???",
        "DW_AT_discr",              /* 0x15 */
        "DW_AT_discr_value",        /* 0x16 */
        "DW_AT_visibility",         /* 0x17 */
        "DW_AT_import",             /* 0x18 */
        "DW_AT_string_length",      /* 0x19 */
        "DW_AT_common_reference",    /* 0x1a */
        "DW_AT_comp_dir",           /* 0x1b (CU) string */
        "DW_AT_const_value",        /* 0x1c */
        "DW_AT_containing_type",    /* 0x1d */
        "DW_AT_default_value",      /* 0x1e */
        "???",
        "DW_AT_inline",             /* 0x20 */
        "DW_AT_is_optional",        /* 0x21 */
        "DW_AT_lower_bound",        /* 0x22 */
        "???",
        "???",
        "DW_AT_producer",           /* 0x25 (CU) string */
        "???",
        "DW_AT_prototyped",         /* 0x27 */
        "???",
        "???",
        "DW_AT_return_addr",        /* 0x2a */
        "???",
        "DW_AT_start_scope",        /* 0x2c */
        "???",
        "DW_AT_bit_stride",         /* 0x2e */
        "DW_AT_upper_bound",        /* 0x2f */
        "???",
        "DW_AT_abstract_origin",    /* 0x31 */
        "DW_AT_accessibility",      /* 0x32 */
        "DW_AT_address_class",      /* 0x33 */
        "DW_AT_artificial",         /* 0x34 */
        "DW_AT_base_types",         /* 0x35 (CU) reference */
        "DW_AT_calling_convention", /* 0x36 */
        "DW_AT_count",              /* 0x37 */
        "DW_AT_data_member_location",/* 0x38 */
        "DW_AT_decl_column",        /* 0x39 */
        "DW_AT_decl_file",          /* 0x3a */
        "DW_AT_decl_line",          /* 0x3b */
        "DW_AT_declaration",        /* 0x3c */
        "DW_AT_discr_list",         /* 0x3d */
        "DW_AT_encoding",           /* 0x3e */
        "DW_AT_external",           /* 0x3f */
        "DW_AT_frame_base",         /* 0x40 */
        "DW_AT_friend",             /* 0x41 */
        "DW_AT_identifier_case",    /* 0x42 (CU) constant */
        "DW_AT_macro_info",         /* 0x43  macptr (CU) */
        "DW_AT_namelist_item",      /* 0x44  block */
        "DW_AT_priority",           /* 0x45  reference */
        "DW_AT_segment",            /* 0x46  block, loclistptr */
        "DW_AT_specification",      /* 0x47  reference */
        "DW_AT_static_link",        /* 0x48  block, loclistptr */
        "DW_AT_type",               /* 0x49  reference */
        "DW_AT_use_location",       /* 0x4a  block, loclistptr */
        "DW_AT_variable_parameter", /* 0x4b  flag */
        "DW_AT_virtuality",         /* 0x4c  constant */
        "DW_AT_vtable_elem_location",/* 0x4d  block, loclistptr */
        "DW_AT_allocated",          /* 0x4e  block, constant, reference */
        "DW_AT_associated",         /* 0x4f  block, constant, reference */
        "DW_AT_data_location",      /* 0x50  block */
        "DW_AT_byte_stride",        /* 0x51  block, constant, reference */
        "DW_AT_entry_pc",           /* 0x52  address */
        "DW_AT_use_UTF8",           /* 0x53  flag */
        "DW_AT_extension",          /* 0x54  reference */
        "DW_AT_ranges",             /* 0x55  rangelistptr */
        "DW_AT_trampoline",         /* 0x56  address, flag, reference, string */
        "DW_AT_call_column",        /* 0x57  constant */
        "DW_AT_call_file",          /* 0x58  constant */
        "DW_AT_call_line",          /* 0x59  constant */
        "DW_AT_description",        /* 0x5a  string */
        "DW_AT_binary_scale",       /* 0x5b  constant */
        "DW_AT_decimal_scale",      /* 0x5c  constant */
        "DW_AT_small",              /* 0x5d  reference */
        "DW_AT_decimal_sign",       /* 0x5e  constant */
        "DW_AT_digit_count",        /* 0x5f  constant */
        "DW_AT_picture_string",     /* 0x60  string */
        "DW_AT_mutable",            /* 0x61  flag */
        "DW_AT_threads_scaled",     /* 0x62  flag */
        "DW_AT_explicit",           /* 0x63  flag */
        "DW_AT_object_pointer",     /* 0x64  reference */
        "DW_AT_endianity",          /* 0x65  constant */
        "DW_AT_elemental",          /* 0x66  flag */
        "DW_AT_pure",               /* 0x67  flag */
        "DW_AT_recursive",          /* 0x68  flag */
        "DW_AT_signature",          /* 0x69  reference (Dwarf4) */
        "DW_AT_main_subprogram",    /* 0x6a  flag (Dwarf4) */
        "DW_AT_data_bit_offset",    /* 0x6b  constant (Dwarf4) */
        "DW_AT_const_expr",         /* 0x6c  flag (Dwarf4) */
        "DW_AT_enum_class",         /* 0x6d  flag (Dwarf4) */
        "DW_AT_linkage_name",       /* 0x6e string (Dwarf4) */
    };

    /*
     *  ======== abbrevRecArr ========
     *  Abbreviation Records are read once into this array  
     */
    private Dwarf32.Abbrev[] abbrevRecArr = null;

    /*
     *  ======== globalMap ========
     *  HashMap of global variables  
     *
     *  Stores name of the variables and its type address
     */
    private HashMap<String, Integer> globalMap = null;

    /*
     *  ======== typesMap ========
     *  HashMap of all data type information
     *
     *  Stores type address and if it is not the base type then the address 
     *  of it's type.
     */
    private HashMap<Integer, Types> typesMap = null;

    /*
     *  ======== Dwarf32 ========
     *  Constructor
     */
    public Dwarf32()
    {
        globalMap = new HashMap<String, Integer>();
        typesMap  = new HashMap<Integer, Types>();
    }

    /*
     *  ======== Variable ========
     *  Class spec to hold Variable information
     */
    public class Variable
    {
        public String name;    /* Variable Name */
        public String type;    /* Base type of the variable */
    }

    /*
     *  ======== Types ========
     *  Class spec to hold Type information  
     */
    private class Types
    {
        public String name;   /* Name of the type */
        public int type;      /* Address to base type (if any) */
    }

    /*
     *  ======== CUHeader ========
     *  Class spec to hold Compilation Unit Header information  
     */
    private class CUHeader
    {
        public int   unit_length;          /* length of compilation unit */
        public short version;               
        public int   debug_abbrev_offset;  /* offset into abbrev rec section */
        public byte  address_size;

        public void readCUHeader(ByteBuffer buf) {
           unit_length = buf.getInt();
           version = buf.getShort();
           debug_abbrev_offset = buf.getInt();
           address_size = buf.get();
           forms[DW_FORM_addr].size = address_size;
        }
    }

    /*
     *  ======== DieAttr ========
     *  Class spec to hold DIE attribute information  
     */
    private class DieAttr
    {
        public int id;
        public int form;
    }

    /*
     *  ======== Abbrev ========
     *  Class spec to hold Abbreviation record information  
     */
    private class Abbrev {
        public int             offset;    /* offset of DIE in the abbrev rec */  
        public int             code;      /* info that links various recs */
        public int             tag;       /* DIE type */
        public boolean         children;  /* Does this DIE have children? */
        public Dwarf32.DieAttr[] attrs;   /* variable num of Dwarf32_DieAttr */

        public Abbrev() {
            offset = 0;
            code = 0;
            tag =  0;
            children = false;
            attrs = null;
        }
    }

    /*
     *  ======== getAbbrevRec ========
     *  Get the abbreviation record 
     *
     *  Get the abbreviation for the given abbreviation code starting at the 
     *  abbreviation offset.
     *
     *  Note: This function can be further optimized using binary search.
     */
    private Dwarf32.Abbrev getAbbrevRec(int abbrevOffset, int code)
        throws Exception
    {
        Dwarf32.Abbrev abbrevRec = null;

        if (abbrevRecArr == null) {
            throw new Exception("Abbreviation record array is null");
        }
        
        for (int j = 0; j < abbrevRecArr.length; j++) {
            abbrevRec = abbrevRecArr[j];
            
            /* Skip till the offset */
            if (abbrevRec.offset == abbrevOffset) {
                /* Now lets check the code */
                for (; j < abbrevRecArr.length; j++) {
                    abbrevRec = abbrevRecArr[j];

                    if (abbrevRec.code == code) {
                        break;
                    }
                }
                break;
            }
        }

        /* Something bad happened */
        if (abbrevRec == null || abbrevRec.code != code) {
            String errorMsg = "Failed to find abbrev code " + code +
                               " from offset " + abbrevOffset; 
            throw new Exception(errorMsg);
        }

        return (abbrevRec);
    }
      
    /*
     *  ======== getAttrName ========
     *  Get the DW_AT_* string
     */
    private String getAttrName(int attr)
    {
        if (attr < attrNames.length) {
            return (attrNames[attr]);
        }
        else if (attr >= DW_AT_lo_user && attr <= DW_AT_hi_user) {
            return ("DW_AT_user_" + Integer.toHexString(attr));
        }
        else {
            return ("<unknown DW_AT value: " + Integer.toHexString(attr) + ">");
        }
       
    }

    /*
     *  ======== getBaseType ========
     *  Recursively navigate to get the base type name
     */
    private String getBaseType(HashMap<Integer, Types> typesMap, int type)
    {
        Types typeObj = typesMap.get(type);

        if (typeObj == null) {
            return ("");
        }

        int tempType = typeObj.type;
        if (tempType != 0) {
            return (this.getBaseType(typesMap, tempType));
        }
       
        return (typesMap.get(type).name);
    }
     
    /*
     *  ======== getGlobalVariables ========
     *  Get all the global variables and their type information
     */
    public Variable[] getGlobalVariables() throws Exception
    {
         return (this.getGlobalVariablesByType(".*"));
    }

    /*
     *  ======== getGlobalVariablesByType ========
     *  Get global variables of particular types matching the regex
     *
     *  @params(typeRegEx)  Regular expression to limit types of variables
     */
    public Variable[] getGlobalVariablesByType(String typeRegEx) 
        throws Exception
    {
        ArrayList<Variable> varList = new ArrayList<Variable>();

        /* Populate an array of Variables with name and type info */ 
        for (Map.Entry<String, Integer> entry: globalMap.entrySet()) {
            String varName = entry.getKey();
            String varType = this.getBaseType(typesMap, entry.getValue());

            /* Skip the junk symbols */
            if (varName == "" || varType == "") {
                continue;
            }
                
            /* save the info in the array */
            if (varType.matches(typeRegEx)) {
                Variable var = new Variable();
                var.name = varName;
                var.type = varType;
                varList.add(var);
            }
        }

        return (varList.toArray(new Variable[varList.size()]));
    }

    /*
     *  ======== getTagName ========
     *  Get the DW_TAG_* string
     */
    private String getTagName(int tag)
    {
        if (tag < tagNames.length) {
            return (tagNames[tag]);
        }
        else if (tag >= DW_TAG_lo_user && tag <= DW_TAG_hi_user) {
            return ("DW_TAG_user_"+ Integer.toHexString(tag));
        }
        else {
            return ("<unknown DW_TAG value: " + Integer.toHexString(tag) + ">");
        } 
    }

    /*
     *  ======== parse ========
     *  Parse dwarf information to get global variables info
     *
     *  Parses the .debug_info, .debug_abbrev and .debug_str sections of
     *  DWARF data to populate a list of variables and their types.
     *
     *  @params(info)   byte buffer containing .debug_info section.
     *  @params(abbrev) byte buffer containing .debug_abbrev section.
     *  @params(str)    byte buffer containing .debug_str section. Certain code
     *                  gen tools do not support this section. So this can be
     *                  null.
     */
    public void parse(ByteBuffer info, ByteBuffer abbrev, ByteBuffer str)
        throws Exception
    {
        if (info == null) {
            throw new Exception("Cannot read .debug_info");
        }

        if (abbrev == null) {
            throw new Exception("Cannot read .debug_abbrev");
        }

        /* read abbrevation record into a buffer */
        readAbbrevRecArr(abbrev);
            
        /* Decode the .debug_info section */
        while (info.remaining() > 0) {
            CUHeader header = new CUHeader();
            int abbrevCode = 0;
            Dwarf32.Abbrev abbrevRec = null;

            /* Compilation unit tracker */
            int cuOffset = info.position();

            /* Get compilation unit information from the header */
            header.readCUHeader(info);

            /* get abbreviation code (record offset) */
            abbrevCode = this.readULEB128(info);
                
            /* Get the compilation unit DIE abbreviation record */
            abbrevRec = this.getAbbrevRec(header.debug_abbrev_offset,
                                          abbrevCode);
            
            /* loop through CU DIE. We don't need this info*/
            for (int k = 0; k < abbrevRec.attrs.length; k++)
            {
                int id = abbrevRec.attrs[k].id;
                int form = abbrevRec.attrs[k].form;
                int size = forms[form].size;

                if (size > 0) {
                    if ((id == DW_AT_comp_dir) || (id == DW_AT_name) 
                        || (id == DW_AT_stmt_list)){
                        info.getInt();
                    }
                    else {
                        info.position(info.position() + size);
                    }
                }
                else if (size == 0) {
                    if (form == DW_FORM_string) {
                        this.readStringFromBuffer(info, info.position());
                    }
                    else {
                        this.readULEB128(info);
                    }
                }
                else {
                    throw new Exception("Size cannot be < 0 : " + size);
                }
            }
           
            /* Looping through all the DIEs */
            int level = 1; /* Keep track of the symbol scope */
            for (int j = 0; 
                 info.position() < (cuOffset + header.unit_length + 4); j++) {
                   
                /* Keep track of DIE position */
                int dieOffset = info.position();

                /* flags for info extraction */                    
                boolean isVar = false;
                boolean isType = false;
                    
                /* temporary storage */
                String atName = "";
                String atType = "0";
                Types typeObj = null;

                abbrevCode = this.readULEB128(info);
                   
                /* if it is a null DIE, skip it */
                if (abbrevCode == 0) {
                    if (level > 1 ) {
                        level--;
                    }
                    continue;
                }
 
                abbrevRec = this.getAbbrevRec(header.debug_abbrev_offset, abbrevCode);
                    
                /* Is it either a variable or type DIE? */
                String tagName = getTagName(abbrevRec.tag);
                if (tagName.matches("DW_TAG_variable")) {
                    isVar = true;
                }
                else if (tagName.matches(".*type.*")) {
                    isType = true;
                    typeObj = new Types();
                }

                /* Loop through DIE attributes */ 
                for (int k = 0; k < abbrevRec.attrs.length; k++) {
                    int form = abbrevRec.attrs[k].form;
                    int size = forms[form].size;
                    String name = getAttrName(abbrevRec.attrs[k].id);
                    String value = "";
                         
                    if (size > 0) {
                        if (form == DW_FORM_strp) {
                            if (str == null) {
                                throw new Exception("DW_FORM_strp shouldn't appear with null string table");
                            }
                            value = this.readStringFromBuffer(str, info.getInt());
                        }
                        else if (form == DW_FORM_ref1) {
                            value = Integer.toString(cuOffset + info.get());
                        }
                        else if (form == DW_FORM_ref2) {
                            value = Integer.toString(cuOffset + info.getShort());
                        }
                        else if (form == DW_FORM_ref4) {
                            value = Integer.toString(cuOffset + info.getInt());
                        }
                        else if (form == DW_FORM_ref_addr) {
                            value = Integer.toString(info.getInt());
                        }
                        else {
                            int bsize = 0;
                            switch (size) {
                                case 1: { 
                                    bsize = info.get();
                                    break;
                                }
                                case 2: {
                                    bsize = info.getShort();
                                    break;
                                }
                                case 4: {
                                    bsize = info.getInt();
                                    break;
                                }
                                default: {
                                    /* Should not happen? */
                                    throw new Exception(name + "has an unsupported size: " +  size);
                                }
                            }

                            value = Integer.toString(bsize);

                            if (form == DW_FORM_block1 || form == DW_FORM_block2
                                || form == DW_FORM_block4) {
                                info.position(info.position() +  bsize);
                            }
                        }
                    }
                    else if (size == 0) {
                        switch (form) {
                            case DW_FORM_string: {
                                value = this.readStringFromBuffer(info, info.position());
                                break;
                            }
                            case DW_FORM_block: {
                                int bsize = readULEB128(info);
                                value = ""; /* just to be sure */
                                while (bsize != 0) {
                                    /* equivalent to integer shifting */
                                    value += Integer.toString(info.get());
                                    bsize--;
                                }
                                break;
                            }
                            case DW_FORM_sdata: {
                                int formVal = readSLEB128(info);
                                value = Integer.toString(formVal);
                                break;
                            }
                            default: {
                                int formVal = readULEB128(info);
                                if (form == DW_FORM_udata) {
                                    value = Integer.toString(formVal);
                                }
                                else {
                                    value = Integer.toString(cuOffset + formVal);
                                }
                                break;
                            }
                        }
                    }
                    else {
                        /* Should not happen! */
                        throw new Exception(name + " has an  invalid form: " + form);
                    }
                        
                    /* Save name and type info in temporary variables */ 
                    if (name == "DW_AT_name") {
                        atName = value;
                    }
                    else if (name == "DW_AT_type") {
                        atType = value;
                    }

                }
                if (abbrevRec.children) {
                    level++;
                }
                    
                /* Store variables if it is global(based on level) */
                if (isVar && (level == 1) && atName != "") {
                    globalMap.put(atName, Integer.parseInt(atType));
                }
                else if (isType) {
                    if (atType == "") {
                        atType = "0";
                    }

                    typeObj.name = atName;
                    typeObj.type = Integer.parseInt(atType);
                    typesMap.put(dieOffset, typeObj);
                }
            }
        }

    }

    /*
     *  ======== readAbbrevRecArr ========
     *  Parse the abbreviation record information
     *
     *  The ByteBuffer containing abbreviation records section is parsed
     *  and put in a global array to be accessed later.
     */
    private void readAbbrevRecArr(ByteBuffer abbrev) throws Exception
    {
        ArrayList<Dwarf32.Abbrev> arecList = new ArrayList<Dwarf32.Abbrev>();

        while (abbrev.remaining() > 0) {
            Dwarf32.Abbrev arec = new Dwarf32.Abbrev();
            ArrayList<Dwarf32.DieAttr> dieAttrList = new ArrayList<Dwarf32.DieAttr>();

            arec.offset = abbrev.position();
            arec.code = this.readULEB128(abbrev);   
                 
            if (arec.code != 0 && abbrev.remaining() > 0) {
                arec.tag = this.readULEB128(abbrev);
                arec.children = abbrev.get() == 0 ? false : true;

                while (abbrev.remaining() > 0) {
                    Dwarf32.DieAttr dieAttr = new Dwarf32.DieAttr();
                    dieAttr.id = this.readULEB128(abbrev);
                    dieAttr.form = this.readULEB128(abbrev);
                    
                    /* dieAttr ends with a zeroed id and form */
                    if (dieAttr.id == 0 && dieAttr.form == 0) {
                        break;
                    }
                    dieAttrList.add(dieAttr);

                }
                arec.attrs = dieAttrList.toArray(new Dwarf32.DieAttr[dieAttrList.size()]);
            }
            arecList.add(arec);
        }
         
        /* Store it away in global array */
        abbrevRecArr = (arecList.toArray(new Dwarf32.Abbrev[arecList.size()]));
    }

    /*
     *  ======== readStringFromBuffer ========
     */
    private String readStringFromBuffer(ByteBuffer buffer, int offset)
    {
        /* Move the ByteBuffer's position to the specified offset. */
        buffer.position(offset);

        /* Walk through the buffer until we find the null terminator. */
        while (buffer.get() != 0) {
        }

        /*
         * Calculate the length of the string based on the current position
         * (the char after the null terminator).
         */
        int strLen = buffer.position() - offset - 1;

        /* Move the buffer back to the location of the string. */
        buffer.position(offset);

        /* Allocate a byte[] to hold the string. */
        byte [] stringBuf = new byte[strLen];

        /* Copy the characters from the ByteBuffer into stringBuf. */
        buffer.get(stringBuf, 0, strLen);

        /* remove null char */
        buffer.get();

        /* Create a String from the buffer and return it. */
        return (new String(stringBuf, 0, strLen));
    }
 
    /*
     *  ======== readSLEB128 ========
     *  Read Signed Little Endian Base 128 data
     *
     *  Note: Read DWARF Spec for more info on LEB128
     */
    private int readSLEB128(ByteBuffer buf)
    {
        byte byt = 0;
        int  result = 0;
        int  shift = 0;
        int  size = 32;       /* number of bits */
        
        /* 32 bit can be spread across 5 bytes: 4 7 7 7 7 bits */
        for (int i = 0; i < 5; i++, shift +=7) {
            byt = buf.get();
            result |= (0x7f & byt) << shift;
            if ((0x80 & byt) == 0) {
                break;
            }  
        }
        /* sign padding */
        if ((shift < 32) && (0x40 & byt) == 1) {
            result |= - (1 << shift);
        }

        return (result);           
    }

    /*
     *  ======== readULEB128 ========
     *  Read Unsigned Little Endian Base 128 data
     *
     *  Note: Read DWARF Spec for more info on LEB128
     */
    private int readULEB128(ByteBuffer buf)
    {
        byte byt = 0;
        int  result = 0;
        int  shift = 0;

        /* 32 bit can be spread across 5 bytes: 4 7 7 7 7 bits */
        for (int i = 0; i < 5; i++, shift +=7) {
            byt = buf.get();
            result |= (0x7f & byt) << shift;
            if ((0x80 & byt) == 0) {
                break;
            }  
        }
        
        return (result);
    }

    /*
     *  ======== printVariables ========
     *  Prints all the variables from the array
     */
    public void printVariables(Variable vars[])
    {
        for (int varIndex = 0; varIndex < vars.length; varIndex++) {
            System.out.println("(" + (varIndex + 1) + ")\n"
                              + "    name: " + vars[varIndex].name + "\n"
                              + "    type: " + vars[varIndex].type + "\n");
        }
    }

    /*
     *  ======== main ========
     *  Unit test for this class
     *
     *  Note: The below code is commented to remove dependency on 
     *        ELF. For unit testing, this code can be uncommented.
     */
    public static void main(String args[]) throws Exception
    {
        if ((args.length != 1)) {
            System.out.println("Usage: dwarf elffile\n"
                              +"       dwarf a.out");
            return;
        }
        
        Elf32 elf = new Elf32();
        elf.parse(args[0]);
        Dwarf32 dw = new Dwarf32();
        Dwarf32.Variable[] vars = elf.getGlobalVariablesByType(".*_Struct");
        dw.printVariables(vars);
    }
}
