/**
 * Object Table
 *
 * DEFINE_OBJECT should be used for normal objects
 *    - Argument 1: Name of the object segment in spec
 *    - Argument 2: Enum value for this object
 *
 * DEFINE_OBJECT_UNSET and DEFINE_OBJECT_NULL are needed to define empty entries from the original game
 */
/* 0x0000 */ DEFINE_OBJECT_UNSET(OBJECT_INVALID) // Object ID 0 isn't usable and should remain unset
/* 0x0001 */ DEFINE_OBJECT(gameplay_keep, OBJECT_GAMEPLAY_KEEP)
/* 0x0002 */ DEFINE_OBJECT(gameplay_field_keep, OBJECT_GAMEPLAY_FIELD_KEEP)
/* 0x0003 */ DEFINE_OBJECT(gameplay_dangeon_keep, OBJECT_GAMEPLAY_DANGEON_KEEP)


/* 0x000E */ DEFINE_OBJECT(object_box, OBJECT_BOX) ///////////////////
/* 0x0014 */ DEFINE_OBJECT(object_link_boy, OBJECT_LINK_BOY)
/* 0x0015 */ DEFINE_OBJECT(object_link_child, OBJECT_LINK_CHILD)
             DEFINE_OBJECT(object_jya_obj, OBJECT_JYA_OBJ)

/* 0x00E0 */ DEFINE_OBJECT(object_ma1, OBJECT_MA1) //////////



