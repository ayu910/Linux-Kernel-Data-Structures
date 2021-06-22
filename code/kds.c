/*
 * Description:         A module for implementation of kernel data structures
 * Version:             1.0
 * Author:              Ayush Sharma
*/

#include <linux/hashtable.h>
#include <linux/syscalls.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/kernel.h> 
#include <linux/init.h> /* Init and exit macros */

#define ODD 1
#define MAX_ITEMS 7
#define SIZE 1024

/* Declare global bitmap */
static DECLARE_BITMAP(bitmap, SIZE);


struct header
{
	DECLARE_HASHTABLE(num_hasher, 14);
};



/* Command Line arguments for module */
static char *int_str = "Some Garbage Value";
module_param(int_str, charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(int_str, "Another parameter, a string");


/* Description 	: 	Linked List function takes an integer array and 
 * 			creates a linked list. It ietrates and deletes 
 * 			the elements of list. 
 * Parameters 	: 	An integer array and array size.
 * Returns 	:	Return value is 0 on success. Non zero on failure.	
 */
static int linked_list_fn(int int_arr[], int arr_size)
{
        int empty = 0, i = 0;
	/*list structure*/
        struct numbers {
                struct list_head list1; 
                int int_value;
        };
        struct numbers *my_num2, *my_num3, *my_num4;
        struct numbers *my_num = kmalloc(sizeof(*my_num), GFP_KERNEL);
        INIT_LIST_HEAD(&my_num->list1); /* initialize an element */


        printk(KERN_INFO "\n\n******************************** Linked List Implementation *******************************\n");

	/* Add array values to the list */
	printk("Adding values to linked list.");
	for(i = 0; i < arr_size; i++)
	{
                my_num2 = kmalloc(sizeof(*my_num2), GFP_KERNEL);
                my_num2->int_value = int_arr[i];
                list_add_tail(&my_num2->list1, &my_num->list1);
        }
	/* Check if list is empty */
        empty = list_empty(&my_num->list1);
	if(empty != 0)
	{
		printk("List is empty. Exiting with error.");
		return empty;
	}

	/* Iterate through list */
	printk("Iterating through values in linked list.");
        list_for_each_entry(my_num4, &my_num->list1, list1)
        {
                printk("Next Number in list is : %d", my_num4->int_value);
	}

	/* Iterate through list and delete list entries */
	printk("Deleting values in linked list.");
	list_for_each_entry_safe(my_num4, my_num3, &my_num->list1, list1)
        {
                printk("Deleting from list : %d", my_num4->int_value);
                list_del(&my_num4->list1);
                kfree(my_num4);
        }

	/* Check if list is empty */
	printk("Checking if linked list is empty and exiting.");
        empty = list_empty(&my_num->list1);
	if(empty == 0)
        {
                printk("List is not empty. Exiting with error.");
                return empty;
        }
	return 0;
}



/* Description 	: 	Red Black Tree function takes an integer array and 
 * 			creates a red black tree. It iterates and deletes 
 * 			the elements of rb tree. 
 * Parameters 	: 	An integer array and array size.
 * Returns 	:	Return value is 0 on success. Non zero on failure.	
 */
static int red_black_tree_fn(int int_arr[], int arr_size)
{
	int insert_val, i, count;
	struct rb_root num_tree;
	struct item 
	{
		struct rb_node node;
		int val;
	};

	struct item *currItem, *insert_item, *read_item;

	struct rb_node **curr = &num_tree.rb_node;
	struct rb_node *parent = NULL;
	struct rb_node *new_parent = NULL;
	
	/* Initialize rbtree root */
	num_tree = RB_ROOT;

	printk(KERN_INFO "\n\n******************************** Red Black Tree Implementation *******************************\n");

	printk("Inserting values in RBTree.");
	/* Inserting values in rbtree*/
	for (i = 0; i < arr_size; i++) {
		insert_val = int_arr[i];
		insert_item = kmalloc(sizeof(struct item*), GFP_KERNEL);
		insert_item->val = insert_val;

		printk("Inserting item %d", insert_val);

		while(*curr) {
			parent = *curr;
			currItem = rb_entry(parent, struct item, node);
			if (insert_item->val < currItem->val) {
				curr = &parent->rb_left;
			} else {
				curr = &parent->rb_right;
			}
		}

		rb_link_node(&insert_item->node, parent, curr);
		rb_insert_color(&insert_item->node, &num_tree);
	}

	count = 0;
	/* Iterating through rbtree and printing values.*/
	printk("Reading values from RBTree");
	for (parent = rb_first(&num_tree); parent; parent = rb_next(parent)) {
		read_item = rb_entry(parent, struct item, node);
		printk("Val in read RBTree item %d", read_item->val);
		count++;
	}
	printk("Total count of values in RBTree before deletion %d", count);

	printk("Deleting items in RBTree");
	/* Iterating through rbtree and deleting values */
	for (parent = rb_first(&num_tree); parent; parent = new_parent) {
                read_item = rb_entry(parent, struct item, node);
		new_parent = rb_next(parent);
                printk("Deleting item %d in RBTree", read_item->val);
		rb_erase(&read_item->node, &num_tree);
                count++;
        }

	count = 0;
	/* Iterating through rbtree after deletion to check count is 0 */
	for (parent = rb_first(&num_tree); parent; parent = rb_next(parent)) {
                read_item = rb_entry(parent, struct item, node);
                printk("Val in read RBTree item %d", read_item->val);
                count++;
        }
	printk("Total count of values in RBTree after deletion %d", count);

	return 0;
}

/* Description 	: 	Hash function takes an integer array and 
 * 			creates a hash map. It iterates and deletes 
 * 			the elements of hash. 
 * Parameters 	: 	An integer array and array size.
 * Returns 	:	Return value is 0 on success. Non zero on failure.	
 */
static int hash_fn(int int_arr[], int arr_size)
{
	int result2;
	int i = 0;
	struct numbers 
	{
		struct hlist_node list1;
		int integer_value;
	};
	struct numbers *my_num, *my_num1, *my_num2;
	struct hlist_node *temp_node;
	struct header *num_hasher_ptr = kmalloc(sizeof(struct header)*2, GFP_KERNEL);


	/* Initializing Hash */
	hash_init(num_hasher_ptr->num_hasher);
	
	printk(KERN_INFO "\n\n******************************** Hash Implementation *******************************\n");
		
	printk("Adding values in the hash table.");
	/* Adding values in the hash table with key = int_value to be inserted */
	for(i = 0; i< arr_size; i++)
	{
		my_num = kmalloc(sizeof(struct numbers *), GFP_KERNEL);
		my_num->integer_value = int_arr[i];
		hash_add(num_hasher_ptr->num_hasher, &my_num->list1, int_arr[i]);
		printk("Number added :%d", my_num->integer_value);
	}
	
	/* Iterating the hash table and printing values */
	printk("Printing values in the hash table.");
	hash_for_each(num_hasher_ptr->num_hasher,result2, my_num1, list1 )
	{		
		printk("Next number in hash is : %d", my_num1->integer_value);
		break;
	}
	
	/* Looking up the hash table using key and printing the value. Deleting hash table elements.*/
	printk("Looking up numbers using key, printing and deleting values from hash.");
	for(i = 0; i< arr_size; i++)
	{	
		hash_for_each_possible_safe(num_hasher_ptr->num_hasher, my_num2, temp_node, list1, int_arr[i] )
		{
			printk("Value is : %d. Deleting this value.", my_num1->integer_value);
			hash_del(&my_num1->list1);
			kfree(my_num1);
			break;
		}
	}
	return 0;
}

/* Description 	: 	Radix Tree function takes an integer array and 
 * 			creates a radix tree. It iterates and deletes 
 * 			the elements of radix tree. 
 * Parameters 	: 	An integer array and array size.
 * Returns 	:	Return value is 0 on success. Non zero on failure.	
 */
static int radix_tree_fn(int int_arr[], int arr_size)
{
	int i, elem_count;
	int first_index = 0;
	int *results[MAX_ITEMS];
	
	/* Initialise radix tree */
	RADIX_TREE(rdxtree, GFP_KERNEL);
	

        printk(KERN_INFO "\n\n******************************** Radix Tree Implementation *******************************\n");
	
	/* Adding items in radix tree and tagging the odd ones */
	printk("Adding items in the radix tree.");
	for (i = 0; i < arr_size; i++) 
	{
		radix_tree_preload(GFP_KERNEL);
		radix_tree_insert(&rdxtree, int_arr[i], int_arr + i);
		radix_tree_preload_end();
		if (int_arr[i] % 2 != 0)
			radix_tree_tag_set(&rdxtree, int_arr[i], ODD);
	}

	/* Looking up the inserted numbers and printing */
	elem_count = radix_tree_gang_lookup(&rdxtree, (void **) results, first_index, arr_size);
	printk("Total count of numbers in the radix tree: %d", elem_count);
	for (i = 0; i < elem_count; i++) {
		printk("Next number in radix tree is : %d", *results[i]);
	}

	/* Getting number of odd tagged items and also getting the odd numbers in the results array */
	elem_count = radix_tree_gang_lookup_tag(&rdxtree, (void **) results, first_index, arr_size, ODD);
	printk("Total count of odd numbers in the radix tree: %d", elem_count);
	for (i = 0; i < elem_count; i++) {
		printk("Next odd number in radix tree is : %d", *results[i]);
	}
	
	/* Deleting all indices */
	printk("Deleting elements from radix tree.");
	for (i = 0; i < arr_size; i++) {
		radix_tree_delete(&rdxtree, int_arr[i]);
	}

	elem_count = radix_tree_gang_lookup(&rdxtree, (void **) results, first_index, arr_size);
        printk("Total count of numbers in the radix tree after deletion: %d", elem_count);
	return 0;
}

/* Description 	: 	Xarray function takes an integer array and 
 * 			creates a Xarray. It iterates and deletes 
 * 			the elements of xarray. 
 * Parameters 	: 	An integer array and array size.
 * Returns 	:	Return value is 0 on success. Non zero on failure.	
 */
static int xarray_fn(int int_arr[], int arr_size)
{
	int i, count = 0;
	void *entry;
	unsigned long index;
	/* Initializing xarray */
	DEFINE_XARRAY(rdxtree);

        printk(KERN_INFO "\n\n******************************** XArray Implementation *******************************\n");
	
	/* Adding items in xarray and tagging the odd ones */
	printk("Adding items in the radix tree.");
	for (i = 0; i < arr_size; i++) 
	{
		xa_store(&rdxtree, int_arr[i], int_arr + i, GFP_KERNEL);
		if (int_arr[i] % 2 != 0)
			xa_set_mark(&rdxtree, int_arr[i], ODD);
	}

	/* Iterating over all the entries */
	xa_for_each(&rdxtree, index, entry) {
		printk("XArray entry at index %lu -> %d", index, *((int *) entry));
	}

	/* Iterate over the tagged ones */
	xa_for_each_marked(&rdxtree, index, entry, ODD) {
		printk("XArray ODD marked entry %d", *((int *) entry));
	}

	printk("Checking if XArray is empty before deletion (1 means true and 0 means false) - %d ", xa_empty(&rdxtree));

	/* Deleting all elements in the XArray and destroying it */
	xa_destroy(&rdxtree);

	xa_for_each(&rdxtree, index, entry) {
		count++;
        }
	printk("Total XArray entry count after deletion %d", count);
	printk("Checking if XArray is empty before deletion (1 means true and 0 means false) - %d ", xa_empty(&rdxtree));

	return 0;
}

/* Description 	: 	Bitmap function takes an integer array and 
 * 			creates a bitmap. It iterates and deletes 
 * 			the elements of bitmap. 
 * Parameters 	: 	An integer array and array size.
 * Returns 	:	Return value is 0 on success. Non zero on failure.	
 */
static int bitmap_fn(int int_arr[], int arr_size)
{
	int i;
	unsigned long curr_set_bit;
	
        printk(KERN_INFO "\n\n******************************** Bitmap Implementation *******************************\n");
	
	/* Setting all the random values in the bitmap to 0 */
	bitmap_zero(bitmap, SIZE);

	/* Setting bits at the provided location */
	printk("Setting bits in the bitmap.");
	for (i = 0; i < arr_size; i++) 
	{
		set_bit(int_arr[i], bitmap);
	}

	/* Printing the locations at which the bits are set using the inbuilt iterator for bitmaps */
        printk("Iterating through bitmap to print set bits.");
	for_each_set_bit(curr_set_bit, bitmap, 1024) {
                printk("set bit = %lu", curr_set_bit);
        }	

	/* Clearing bits in the bitmap */
	printk("Clearing bitmap.");
	bitmap_zero(bitmap, SIZE);
	return 0;
}


/* Description 	: 	This is the module init function. 
 * 			It convers a string of integers to 
 * 			int and calls multiple data structure functions.
 * Parameters 	: 	None.
 * Returns 	:	Return value is 0 on success. Non zero on failure.	
 */
static int __init kds_init(void) 
{
	int result;
	int str_length = 0, err = 0, counter = 0;
	int int_arr[MAX_ITEMS];
	char * int_value;
	char * encrypted_string;
	printk(KERN_INFO "\n\nModule loaded ...\n");

	/* Copy string to buffer */
	str_length = strlen(int_str);
	encrypted_string = kmalloc(str_length,GFP_KERNEL);
	strcpy(encrypted_string, int_str);
	
	/* Split the string with delim=" " and convert to integer */
	while((int_value = strsep(&encrypted_string, " ")) != NULL) 
	{	
		err = kstrtouint(int_value, 10, &result);
		if(err < 0)
		{
			return err;
		}
		int_arr[counter] = result;
		counter++;
		printk("Next number in buffer is : %d", result);
	}

	/* Call data structure functions */
	linked_list_fn(int_arr, counter);
	red_black_tree_fn(int_arr, counter);
	hash_fn(int_arr, counter);
	radix_tree_fn(int_arr, counter);
	xarray_fn(int_arr, counter);
	bitmap_fn(int_arr, counter);
	return 0; 
}

/* Description 	: 	This is the module exit function. 
 * Parameters 	: 	None.
 * Returns 	:	No return value.	
 */
static void __exit kds_exit(void)
{
	printk(KERN_INFO "Module exiting ...\n");
}


/* Module Info, Init and Exit fucntions */
module_init(kds_init); 
module_exit(kds_exit); 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ayush Sharma <ayush.sharma@cs.stonybrook.edu");
MODULE_DESCRIPTION("Kernel Data Sturctures Implementation");