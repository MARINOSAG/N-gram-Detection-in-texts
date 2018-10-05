#include "static_heap.h"

using namespace std;

/********************************************************************************************/
//Source: http://stackoverflow.com/questions/19720438/pointer-based-binary-heap-implementation/
/********************************************************************************************/

BinaryMaxHeap::BinaryMaxHeap()
{
	sum = 0;
	root = NULL;
	last = NULL;
}

BinaryMaxHeap::~BinaryMaxHeap()
{
	if(root != NULL)	del(root);
}

void BinaryMaxHeap::clear()
{
	if(root != NULL)	del(root);
	root = NULL;
	sum = 0;
}

void BinaryMaxHeap::del(HeapNode *node)
{
	if(node != NULL)
	{
		del(node->left);
		del(node->right);
		delete(node);
	}
}

HeapNode* BinaryMaxHeap::insert(char *caller, hashTableHeap *hashTable)
{
	HeapNode *found, *node;
	HeapNode* node_to_return = NULL;
	//remove character '|' from the end of string caller
	//caller[strlen(caller)-1] = '\0';

	//Search in heap for caller(DFS Traverse)//
	//found = find(caller);
	//found =NULL;
	//if caller not exist in heap, create a new node
	
		node = new HeapNode(caller);
		HeapNode *current = last;

		//sum += val;
		node->value = 1;
		//strcpy(node->caller,caller);
		//case where heap is empty
		if(root == NULL)
		{
			root = node;
			last = node;
			node->parent = NULL;
			node->left = NULL;
			node->right = NULL;
		}
		//find where the node should be inserted
		else
		{
			//start from the last node and go up until there is no parent or current node is the left chile of parent
			while((current->parent != NULL) && (current == current->parent->right))
			{
				current = current->parent;
			}

			if(current->parent != NULL)
			{
				//if parent has  a right child, find the most left node  of the parent's right subtree and attach node
				if(current->parent->right != NULL)
				{
					current = current->parent->right;
					while(current->left != NULL)		current = current->left;
				}
				//if parent hasn't right child, the new nod become parent's right child
				else
				{
					current = current->parent;
				}
			}
			//if we reach the root, we should add a new level to the tree and the new node will be the left child of the most left node
			else
			{
				while(current->left != NULL)			current = current->left;
			}

		//current is the node in which we will add the new node
		//so we have to decide whether we add to the left or to the right child
		last = node;		//set last to the new node
		if(current->left == NULL)	current->left = node;
		else if(current->right == NULL)	current->right = node;
		else	cout<<"ERROR IN HEAP INSERT"<<endl;
		node->parent = current;
		node->left = NULL;
		node->right = NULL;
		}
		node_to_return = node;
	
	

	//after inserting new node to the bottom of the heap, restore heap property
	while(node->parent != NULL && node->parent->value <= node->value)
	{
		if( (node->parent->value == node->value && strcmp(node->parent->caller,node->caller) > 0))
		{ 
			node_to_return = node->parent;
			move_up(node, hashTable);
			
		}
		node = node->parent;
	}

	return node_to_return;
}

void heapify(BinaryMaxHeap* temp_heap , HeapNode* node, hashTableHeap *hashTable)
{
	while(node->parent != NULL && node->parent->value <= node->value)
	{
		if((node->parent->value < node->value) || (node->parent->value == node->value && strcmp(node->parent->caller,node->caller) > 0))
		{ 
			temp_heap->move_up(node, hashTable);
		}
		node = node->parent;
	}
}

void HeapNode::add_one_value(BinaryMaxHeap * temp_heap, hashTableHeap *hashTable)
{
	value++;
	heapify(temp_heap,this, hashTable);
}

void BinaryMaxHeap::move_up(HeapNode *node, hashTableHeap *hashTable)
{
	//cout<<node->value<<"  "<<node->parent->value<<endl;
	int parent_value = node->parent->value;
	char *parent_caller;
	//if(node->parent->caller != NULL)	parent_caller = new char[strlen(node->parent->caller)+1];
	//strcpy(parent_caller,node->parent->caller);
	parent_caller = node->parent->caller;

	hashTable->change_heapPointer(node->caller, node->parent);
	hashTable->change_heapPointer(node->parent->caller, node);

	node->parent->value = node->value;
	node->parent->caller = node->caller;
	//strcpy(node->parent->caller,node->caller);
	node->value = parent_value;
	node->caller = parent_caller;
	//strcpy(node->caller,parent_caller);
}

//Make a DFS Traverse of the Heap to search for caller*******//
//if caller will be found add value to its value and return 1//
//else return 0												 //
HeapNode* BinaryMaxHeap::find(char *caller_to_find)
{
	HeapNode* found;
	found = find(root,caller_to_find);
	if(found != NULL)
	{
		//sum += value;
		found->value += 1;
		return found;
	}
	else return found;
}

HeapNode* BinaryMaxHeap::find(HeapNode *node, char *caller_to_find)
{
	HeapNode* found = NULL;
	if(node != NULL)
	{
		if(strcmp(node->caller,caller_to_find) == 0)
		{
			found = node;
			return found;
		}
		found = find(node->left,caller_to_find);
		if(found != NULL)	return found;
		found = find(node->right,caller_to_find);
		return found;
	}
	return found;
}
/********************************************************************/

/*void BinaryMaxHeap::top(int percent)
{
	if(root != NULL)
	{
		double current_sum, sum_to_reach;
		char c[100];
		strcpy(c,"empty");
		HeapNode *node_with_temp_max, *temp = new HeapNode(c);			//in every loop this node points to the node with the max value that hasn't already been added to the top percent%
		temp->value = 0;

		current_sum = 0;
		sum_to_reach = (percent*sum)/100;
//cout<<"sum = "<<sum_to_reach<<endl;
		flag_init(root);		//init all nodes with flag == 0;

		while(current_sum < sum_to_reach)
		{
			node_with_temp_max = temp;
			top(root, node_with_temp_max);
			current_sum += node_with_temp_max->value;
			node_with_temp_max->flag = 1;		//change flag of the node with the last max value in order not to include it again in the top percent%

			cout<<"\t"<<node_with_temp_max->caller<<" with value: "<<node_with_temp_max->value<<" ("<<(node_with_temp_max->value/sum)*100<<"%)"<<endl;
		}

		delete(temp);
	}
	else		cout<<"\tempty heap"<<endl;
}*/

string BinaryMaxHeap::top(int k)
{
	if(root != NULL)
	{
		int found_new, current_size=0; 
		found_new = 0;
		double current_found = 0;
		char c[100];
		strcpy(c,"empty");
		HeapNode *node_with_temp_max, *temp_node = new HeapNode(c);			//in every loop this node points to the node with the max value that hasn't already been added to the top percent%
		temp_node->value = 0;
		int buff_size = BUFFER_SIZE;
		char * result = (char*)malloc(buff_size*sizeof(char));
		memset(result ,0,buff_size);


		flag_init(root);		//init all nodes with flag == 0;

		while(current_found < k)
		{
			current_found++;
			node_with_temp_max = temp_node;
			top(root, node_with_temp_max, &found_new);
			//current_sum += node_with_temp_max->value;
			node_with_temp_max->flag = 1;		//change flag of the node with the last max value in order not to include it again in the top k

			if(found_new == 1)
			{

				//cout<<"\t"<<node_with_temp_max->caller<<" with value: "<<node_with_temp_max->value<<endl;
				found_new = 0;

				int size=0;
				//for (int i = start;i<=end;i++)
				//{  	
					size +=strlen(node_with_temp_max->caller)+1;
				//}
				current_size += size+1;
				if(current_size > buff_size)//periptwsh gemismatos pinaka result 
				{
					/*char * temp = new char[buff_size];
					memcpy(temp , result , buff_size);
					delete [] result ;//-;
					buff_size *= 2; 
					result = new char[buff_size];
					memset(result, 0, buff_size);
					sprintf(result,"%s",temp);
					delete [] temp;*/
					buff_size*=2;
					result = (char*)realloc(result ,buff_size*sizeof(char) );

				}
				char *temp = new char [buff_size];
				memset(temp,0,buff_size);
				//or(int i = start ;i<=end ; i++)
				//{
						strcat(temp,node_with_temp_max->caller);
						strcat (temp,"|");
				//		if(i != end) strcat(temp, " ");
				//		else strcat (temp,"|");
				//}


				
				char *temp_result = new char[strlen(result)+2];
				sprintf(temp_result,"%s", "|");
				strcat(temp_result,result);
				char *temp3 = new char[strlen(temp)+2];
				sprintf(temp3,"%s","|");
				strcat(temp3,temp);
				if(strstr(temp_result,temp3) == NULL)//an den periexetai sto result hdh
				{	
					strcat(result,temp);
				}
				delete []temp;
				delete [] temp3;
				delete[] temp_result;

			}
			//if there is not any element to heap to print--->break
			else break;
		}

		delete temp_node;
		result[strlen(result)-1] = '\0';
		string output_string(result); 
		free(result);
		return output_string;
	}
	else		cout<<"\tempty heap"<<endl;
}

void BinaryMaxHeap::top(HeapNode *node, HeapNode*& max_node, int *found_new)
{
	if(node != NULL)
	{
			if(((node->value > max_node->value ) || (node->value == max_node->value && strcmp(max_node->caller,node->caller) > 0)) && (node->flag == 0) )
			{
				*found_new = 1;
				max_node = node;
			}
			top(node->left,max_node, found_new);
			top(node->right,max_node, found_new);
	}
}

void BinaryMaxHeap::flag_init(HeapNode *node)
{
	if(node != NULL)
	{
		node->flag = 0;
		flag_init(node->left);
		flag_init(node->right);
	}
}

void BinaryMaxHeap::print()
{
	print(root);
}

void BinaryMaxHeap::print(HeapNode *node)
{
	if(node != NULL)
	{
		print(node->left);
		print(node->right);
		cout<<node->caller<<"  "<<node->value<<endl;
	}
}