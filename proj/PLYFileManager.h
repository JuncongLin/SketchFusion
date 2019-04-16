#pragma once


#include <stdio.h>
#include <stddef.h>

#define PLY_ASCII      1        /* ascii PLY file */
#define PLY_BINARY_BE  2        /* binary PLY file, big endian */
#define PLY_BINARY_LE  3        /* binary PLY file, little endian */

#define PLY_OKAY    0           /* ply routine worked okay */
#define PLY_ERROR  -1           /* error in ply routine */

/* scalar data types supported by PLY format */

#define StartType  0
#define Int8       1
#define Int16      2
#define Int32      3
#define Uint8      4
#define Uint16     5
#define Uint32     6
#define Float32    7
#define Float64    8
#define EndType    9

#define  PLY_SCALAR  0
#define  PLY_LIST    1
#define  PLY_STRING  2

typedef struct PlyProperty {    /* description of a property */

	char *name;                   /* property name */
	int external_type;            /* file's data type */
	int internal_type;            /* program's data type */
	int offset;                   /* offset bytes of prop in a struct */

	int is_list;                  /* 0 = scalar, 1 = list, 2 = char string */
	int count_external;           /* file's count type */
	int count_internal;           /* program's count type */
	int count_offset;             /* offset byte for list count */

} PlyProperty;

typedef struct PlyElement {     /* description of an element */
	char *name;                   /* element name */
	int num;                      /* number of elements in this object */
	int size;                     /* size of element (bytes) or -1 if variable */
	int nprops;                   /* number of properties for this element */
	PlyProperty **props;          /* list of properties in the file */
	char *store_prop;             /* flags: property wanted by user? */
	int other_offset;             /* offset to un-asked-for props, or -1 if none*/
	int other_size;               /* size of other_props structure */
} PlyElement;

typedef struct PlyOtherProp {   /* describes other properties in an element */
	char *name;                   /* element name */
	int size;                     /* size of other_props */
	int nprops;                   /* number of properties in other_props */
	PlyProperty **props;          /* list of properties in other_props */
} PlyOtherProp;

typedef struct OtherData { /* for storing other_props for an other element */
	void *other_props;
} OtherData;

typedef struct OtherElem {     /* data for one "other" element */
	char *elem_name;             /* names of other elements */
	int elem_count;              /* count of instances of each element */
	OtherData **other_data;      /* actual property data for the elements */
	PlyOtherProp *other_props;   /* description of the property data */
} OtherElem;

typedef struct PlyOtherElems {  /* "other" elements, not interpreted by user */
	int num_elems;                /* number of other elements */
	OtherElem *other_list;        /* list of data for other elements */
} PlyOtherElems;

#define AVERAGE_RULE  1
#define MAJORITY_RULE 2
#define MINIMUM_RULE  3
#define MAXIMUM_RULE  4
#define SAME_RULE     5
#define RANDOM_RULE   6


typedef struct PlyPropRules {   /* rules for combining "other" properties */
	PlyElement *elem;      /* element whose rules we are making */
	int *rule_list;        /* types of rules (AVERAGE_PLY, MAJORITY_PLY, etc.) */
	int nprops;            /* number of properties we're combining so far */
	int max_props;         /* maximum number of properties we have room for now */
	void **props;          /* list of properties we're combining */
	float *weights;        /* list of weights of the properties */
} PlyPropRules;

typedef struct PlyRuleList {
	char *name;                  /* name of the rule */
	char *element;               /* name of element that rule applies to */
	char *property;              /* name of property that rule applies to */
	struct PlyRuleList *next;    /* pointer for linked list of rules */
} PlyRuleList;

typedef struct PlyFile {        /* description of PLY file */
	FILE *fp;                     /* file pointer */
	int file_type;                /* ascii or binary */
	float version;                /* version number of file */
	int num_elem_types;           /* number of element types of object */
	PlyElement **elems;           /* list of elements */
	int num_comments;             /* number of comments */
	char **comments;              /* list of comments */
	int num_obj_info;             /* number of items of object information */
	char **obj_info;              /* list of object info items */
	PlyElement *which_elem;       /* element we're currently reading or writing */
	PlyOtherElems *other_elems;   /* "other" elements from a PLY file */
	PlyPropRules *current_rules;  /* current propagation rules */
	PlyRuleList *rule_list;       /* rule list from user */
} PlyFile;



typedef struct Face {
	unsigned char nverts;    /* number of vertex indices in list */
	int *verts;              /* vertex index list */
	void *other_props;       /* other properties */
} Face;

typedef struct Vertex {
	float x,y,z;
	float r,g,b;
	float nx,ny,nz;
	void *other_props;       /* other properties */
} Vertex;


#define myalloc(mem_size) my_alloc((mem_size), __LINE__, __FILE__)

class CPLYFileManager
{
public:
	CPLYFileManager(void);
	~CPLYFileManager(void);
private:
	void ExtractElements(PlyFile* in_ply);
	PlyFile* InterpretHead(FILE* fp);
	char** get_words(FILE* fp, int* nwords, char** orig_line);
	int equal_strings(char* s1, char* s2);
	void add_element(PlyFile* plyfile, char** words, int nwords);
	void add_property(PlyFile* plyfile, char** words, int nwords);
	void add_comment(PlyFile* plyfile, char* line);
	void append_comment_ply(PlyFile* ply, char* comment);
	void add_obj_info(PlyFile* plyfile, char* line);
	void append_obj_info_ply(PlyFile* ply, char* obj_info);
	int get_prop_type(char* type_name);
	char* setup_element_read_ply(PlyFile* ply, int index, int* elem_count);
	void setup_property_ply(PlyFile* plyfile, PlyProperty* prop);
	PlyProperty* find_property(PlyElement* elem, char* prop_name, int* index);
	PlyOtherProp* get_other_properties_ply(PlyFile* plyfile, int offset);
	void get_element_ply(PlyFile* plyfile, void* elem_ptr);
	PlyOtherProp* get_other_properties(PlyFile* plyfile, PlyElement* elem, int offset);
	void ascii_get_element(PlyFile* plyfile, char* elem_ptr);
	void binary_get_element(PlyFile* plyfile, char* elem_ptr);
	void setup_other_props(PlyFile* plyfile, PlyElement* elem);
	void copy_property(PlyProperty* dest, PlyProperty* src);
	void get_ascii_item(char* word, int type, int* int_val, unsigned int* uint_val, double* double_val);
	void get_binary_item(FILE* fp, int type, int* int_val, unsigned int* uint_val, double* double_val);
	void store_item(char* item, int type, int int_val, unsigned int uint_val, double double_val);
	PlyOtherElems* get_other_element_ply(PlyFile* plyfile);
	void close_ply(PlyFile* plyfile);
	void free_ply(PlyFile* plyfile);
	PlyOtherProp* ply_get_other_properties(PlyFile* plyfile, char* elem_name, int offset);
	void ply_get_element(PlyFile* plyfile, void* elem_ptr);
	PlyElement* find_element(PlyFile* plyfile, char* element);
public:
	void Read(FILE* file);
	int GetVertexNum(void);
	int GetFaceNum(void);
private:
	PlyOtherProp *vert_other,*face_other;
	int per_vertex_color ;
	int has_normals ;	
	int nverts,nfaces;
	Vertex **vlist;
	Face **flist;
public:
	void GetVertexByIndex(int index, float& x, float& y, float& z);
	void GetFaceByIndex(int index, int& v1, int& v2, int& v3);
};
