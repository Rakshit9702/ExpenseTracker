#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define FAMILY_MEMBERS 4
#define NAME 100
#define MAX 4
#define MIN 2

typedef enum {
    Rent = 0,
    Grocery,
    Utility,
    Leisure,
    Stationary,
    Others
} ExpenseCategory;

typedef struct {
    int date;
    int month;
} day_and_date;

typedef struct UserNode UserNode;
typedef struct ExpenseNode ExpenseNode;
typedef struct FamilyNode FamilyNode;

typedef struct FamilyNode {
    char name[NAME]; //surname
    int family_id;
    UserNode* member[FAMILY_MEMBERS];
    float total_income;
    float total_expense;
    int member_count;
} FamilyNode;

typedef struct UserNode {
    int user_id;          // user_id
    char user_name[NAME];
    float income; 
    FamilyNode *family;
    //ExpenseNode *expenses[10]; // Pointer to the expense node  
    int exp[10]; // Array to store expense IDs
    int expense_count; // Number of expenses
} UserNode;

typedef struct ExpenseNode {
    int expense_id;          // user_id
    int user_id;
    float amount; 
    ExpenseCategory category;
    day_and_date date;
} ExpenseNode;

typedef struct treenode_user {
    int count;                         
    UserNode entry[MAX + 1];          
    struct treenode_user *branch[MAX + 1];  // Pointers to child nodes
} TreenodeUser;

typedef struct treenode_family{
    int count;                        
    FamilyNode entry[MAX + 1];          
    struct treenode_family *branch[MAX + 1]; 
} TreenodeFamily;

typedef struct treenode_expense{
    int count;                         
    ExpenseNode entry[MAX + 1];       
    struct treenode_expense *branch[MAX + 1];  
} TreenodeExpense;

// Function declarations
TreenodeUser *SearchTree(int target, TreenodeUser *root, int *targetpos);
bool SearchNode(int target, TreenodeUser *current, int *pos);
void PushIn(UserNode medentry, TreenodeUser* medright, TreenodeUser* current, int pos);
void Split(UserNode medentry, TreenodeUser* medright, TreenodeUser* current, int pos, UserNode* newmedian, TreenodeUser** newright);
bool PushDown(UserNode newentry, TreenodeUser* current, UserNode* medentry, TreenodeUser** medright);
TreenodeUser *InsertTree(UserNode newentry, TreenodeUser* root);
void LoadAllData(TreenodeUser** userRoot, TreenodeFamily** familyRoot, TreenodeExpense** expenseRoot);
void PrintTree(TreenodeUser* root);
void SaveAllData(TreenodeUser* userRoot, TreenodeFamily* familyRoot, TreenodeExpense* expenseRoot);
void SaveUsers(TreenodeUser* root, FILE* file);
void SaveFamilies(TreenodeFamily* root, FILE* file);
void SaveMemberships(TreenodeUser* root, FILE* file);
void SaveExpenses(TreenodeExpense* root, FILE* file);
void PrintTreeInOrder(TreenodeUser* root);
TreenodeExpense *SearchTreeExpense(int user_id, int target, TreenodeExpense *root, int *targetpos);
bool SearchNodeExpense(int user_id, int expense_id, TreenodeExpense *current, int *pos);
void PushInExpense(ExpenseNode medentry, TreenodeExpense* medright, TreenodeExpense* current, int pos);
void SplitExpense(ExpenseNode medentry, TreenodeExpense* medright, TreenodeExpense* current, int pos, ExpenseNode* newmedian, TreenodeExpense** newright);
void PushInExpense(ExpenseNode medentry, TreenodeExpense* medright, TreenodeExpense* current, int pos);
bool PushDownExpense(ExpenseNode newentry, TreenodeExpense* current, ExpenseNode* medentry, TreenodeExpense** medright);
void PrintExpensesByUser(TreenodeUser* userRoot, TreenodeExpense* expenseRoot);
TreenodeFamily *SearchTreeFamily(int target, TreenodeFamily *root, int *targetpos);
bool SearchNodeFamily(int target, TreenodeFamily *current, int *pos);
TreenodeFamily *InsertTreeFamily(FamilyNode newentry, TreenodeFamily* root);
bool PushDownFamily(FamilyNode newentry, TreenodeFamily* current, FamilyNode* medentry, TreenodeFamily** medright);
void PushInFamily(FamilyNode medentry, TreenodeFamily* medright, TreenodeFamily* current, int pos);
void SplitFamily(FamilyNode medentry, TreenodeFamily* medright, TreenodeFamily* current, int pos, FamilyNode* newmedian, TreenodeFamily** newright);
void LoadUsers(TreenodeUser** userRoot, FILE* file);
bool CreateFamily(TreenodeFamily** familyRoot, TreenodeUser* root, int user_id, int family_id, char* name, TreenodeExpense* expenseRoot);
void JoinFamily(TreenodeFamily* familyRoot, TreenodeUser* root, int user_id, int family_id, TreenodeExpense* expenseRoot);
void GetIndividualExpense(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int user_id);
int GetTotalExpenseIndividual(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int user_id, int month);
void loadFamilies(TreenodeFamily** familyRoot, FILE* file);
void loadMemberships(TreenodeUser** userRoot, FILE* file);
void loadExpenses(TreenodeExpense** expenseRoot, FILE* file);
void loadUsers(TreenodeUser** userRoot, FILE* file);
void addExpense(TreenodeUser* root, TreenodeExpense** expenseRoot);
void GetMonthlyExpense(TreenodeFamily* familyRoot, TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int family_id);
void GetHighestExpenseDay(TreenodeFamily* familyRoot, TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int family_id);
void GetExpenseInPeriod(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int start_day, int start_month, int end_day, int end_month);
void GetExpenseInRange(TreenodeUser* userRoot, int exp1, int exp2, int user_id, TreenodeExpense* expenseRoot);
UserNode* GetUserById(TreenodeUser* root, int user_id);
float GetTotalExpense(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int user_id);
float GetIndividualExpenseCategory(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int user_id, int category);
void GetCategoricalExpense(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, TreenodeFamily* familyRoot, int family_id, int category);
TreenodeUser *DeleteTreeUser(int target, TreenodeUser *root);
bool DeleteFromTree(int target, TreenodeUser *current);
void RemoveUser(TreenodeUser *current, int pos);
void SuccessorUser(TreenodeUser* current, int pos);
void RestoreUser(TreenodeUser* current, int pos);
void MoveLeftUser(TreenodeUser* current, int pos);
void MoveRightUser(TreenodeUser* current, int pos);
void CombineUser(TreenodeUser* current, int pos);
TreenodeUser *DeleteUser(int target, TreenodeUser *root);
void UpdateUser(TreenodeUser* root, int user_id);
TreenodeFamily *DeleteTreeFamily(int target, TreenodeFamily *root);
bool DeleteFromTreeFamily(int target, TreenodeFamily *current);
void RemoveFamily(TreenodeFamily *current, int pos);
void SuccessorFamily(TreenodeFamily* current, int pos);
void RestoreFamily(TreenodeFamily* current, int pos);
void MoveLeftFamily(TreenodeFamily* current, int pos);
void MoveRightFamily(TreenodeFamily* current, int pos);
void CombineFamily(TreenodeFamily* current, int pos);
TreenodeFamily *DeleteFamily(int target, TreenodeFamily *root);
void Update_Delete_FamilyDetails(TreenodeUser* root, TreenodeExpense* expenseroot, TreenodeFamily* familyRoot, int user_id);
TreenodeExpense *DeleteTreeExpense(int user_id, int expense_id, TreenodeExpense *root);
bool DeleteFromTreeExpense(int user_id, int expense_id, TreenodeExpense *current);
void RemoveExpense(TreenodeExpense *current, int pos);
void SuccessorExpense(TreenodeExpense* current, int pos);
void RestoreExpense(TreenodeExpense* current, int pos);
void MoveLeftExpense(TreenodeExpense* current, int pos);
void MoveRightExpense(TreenodeExpense* current, int pos);
void CombineExpense(TreenodeExpense* current, int pos);
TreenodeExpense *DeleteExpense(int user_id, int expense_id, TreenodeExpense *root);
void deleteUser(TreenodeUser* userRoot, int user_id, TreenodeExpense* expenseRoot, TreenodeFamily* familyRoot);
void deleteFamily(TreenodeFamily* familyRoot, int family_id, TreenodeExpense* expenseRoot);
int AddUser(TreenodeUser** root, TreenodeFamily** familyRoot, TreenodeExpense* expenseRoot);
void addExpense(TreenodeUser* userRoot, TreenodeExpense** expenseRoot);

UserNode CreateUserNode(int user_id, char *name, float income) {
    UserNode newUser;
    newUser.user_id = user_id;
    strcpy(newUser.user_name, name);
    newUser.income = income;
    newUser.expense_count = 0; 
    newUser.family = NULL;  // Initialize family to NULL
    return newUser;
}
FamilyNode CreateFamilyNode(int family_id, char* name, float total_income, float total_expense) {
    FamilyNode newFamily;
    newFamily.family_id = family_id;
    strcpy(newFamily.name, name);
    newFamily.total_income = total_income;
    newFamily.total_expense = total_expense;
    newFamily.member_count = 0;
    
    return newFamily;
}
ExpenseNode CreateExpenseNode(int expense_id, int user_id, float amount, int category, int date, int month) {
    ExpenseNode newExpense;
    newExpense.expense_id = expense_id;
    newExpense.user_id = user_id;
    newExpense.amount = amount;
    newExpense.category = category;
    newExpense.date.date = date;
    newExpense.date.month = month;
    
    return newExpense;
}

TreenodeUser *SearchTree(int target, TreenodeUser *root, int *targetpos) {
    if (!root) 
        return NULL;  

    if (SearchNode(target, root, targetpos))  
        return root;  

    return SearchTree(target, root->branch[*targetpos], targetpos);  
}
bool SearchNode(int target, TreenodeUser *current, int *pos) {
    if (target < current->entry[1].user_id) {  
        *pos = 0;
        return false;
    }
    else {
        for (*pos = current->count; *pos >= 1 && (target < current->entry[*pos].user_id); (*pos)--);
        return (*pos >= 1 && target == current->entry[*pos].user_id);
    }
}

//insert a User(Add a user to the tree)
TreenodeUser *InsertTree(UserNode newentry, TreenodeUser* root) {
    UserNode medentry;
    TreenodeUser* medright;
    TreenodeUser* newroot;
    if (PushDown(newentry, root, &medentry, &medright)) {
        newroot = (TreenodeUser*)malloc(sizeof(TreenodeUser));
        newroot->count = 1;
        newroot->entry[1] = medentry;
        newroot->branch[0] = root;
        newroot->branch[1] = medright;
        return newroot;
    }

    return root;
}
bool PushDown(UserNode newentry, TreenodeUser* current, UserNode* medentry, TreenodeUser** medright) {
    int pos;
    if (current == NULL) {
        *medentry = newentry;
        *medright = NULL;
        return true;
    }
    if (SearchNode(newentry.user_id, current, &pos)) {
        printf("User ID already present\n");
        return false;
    }
    if (PushDown(newentry, current->branch[pos], medentry, medright)) {
        if (current->count < MAX) {
            PushIn(*medentry, *medright, current, pos);
            return false;
        } else {
            Split(*medentry, *medright, current, pos, medentry, medright);
            return true;
        }
    }
    return false;
}
void PushIn(UserNode medentry, TreenodeUser* medright, TreenodeUser* current, int pos) {
    int i;
    for(i = current->count; i > pos; i--) {
        current->entry[i+1] = current->entry[i];
        current->branch[i+1] = current->branch[i];
    }
    current->entry[pos+1] = medentry;
    current->branch[pos+1] = medright;
    current->count++;
}
void Split(UserNode medentry, TreenodeUser* medright, TreenodeUser* current, int pos, UserNode* newmedian, TreenodeUser** newright) {
    int i;
    int median = (pos < MIN) ? MIN : MIN + 1;
    *newright = (TreenodeUser*)malloc(sizeof(TreenodeUser));
    for (i = median + 1; i <= MAX; i++) {
         (*newright)->entry[i - median] = current->entry[i];
         (*newright)->branch[i - median] = current->branch[i];
     }
     (*newright)->count = MAX - median;
     current->count = median;
     if (pos <= MIN)
         PushIn(medentry, medright, current, pos);
     else
         PushIn(medentry, medright, *newright, pos - median);
     *newmedian = current->entry[current->count];
     (*newright)->branch[0] = current->branch[current->count];
     current->count--;
}

void PrintTreeInOrder(TreenodeUser* root) {
    if (root == NULL) {
        return;
    }
    
    int i;
    // Print the subtree rooted at branch[0]
    PrintTreeInOrder(root->branch[0]);
    
    // For each entry in the node
    for (i = 1; i <= root->count; i++) {
        // Print the current entry
        printf("User ID: %d, Name: %s, Income: %.2f\n", 
               root->entry[i].user_id, 
               root->entry[i].user_name, 
               root->entry[i].income);
        
        // Print the subtree rooted at branch[i]
        PrintTreeInOrder(root->branch[i]);
    }
}
void PrintTree(TreenodeUser* root) {
    if (root == NULL) {
        printf("The tree is empty.\n");
        return;
    }
    
    printf("Printing all users in order of ID:\n");
    printf("---------------------------------\n");
    PrintTreeInOrder(root);
    printf("---------------------------------\n");
}

UserNode* GetUserById(TreenodeUser* root, int user_id) {
    int targetPos;
    TreenodeUser* userNodeTree = SearchTree(user_id, root, &targetPos);
    if (userNodeTree == NULL) {
        printf("User with ID %d not found.\n", user_id);
        return NULL;
    }
    // Find the correct user entry in the node
    UserNode* userNode = NULL;
    for (int i = 1; i <= userNodeTree->count; i++) {
        if (userNodeTree->entry[i].user_id == user_id) {
            userNode = &userNodeTree->entry[i];
            return userNode;
        }
    }
    if (userNode == NULL) {
        printf("User with ID %d not found in node.\n", user_id);
        return NULL ;
    }
}
//Searching Expense
TreenodeExpense *SearchTreeExpense(int user_id, int target, TreenodeExpense *root, int *targetpos) {
    if (!root) 
        return NULL;  

    if (SearchNodeExpense(user_id, target, root, targetpos))  
        return root;  

    if(*targetpos <0 || *targetpos > root->count) {
        return NULL;  
    }
    return SearchTreeExpense(user_id, target, root->branch[*targetpos], targetpos);  
}
bool SearchNodeExpense(int user_id, int expense_id, TreenodeExpense *current, int *pos) {
    *pos = current->count;
    
    // userid search karle
    while (*pos >= 1 && user_id < current->entry[*pos].user_id) {
        (*pos)--;
    }
    
    // If same then expense_id search karle
    if (*pos >= 1 && user_id == current->entry[*pos].user_id) {
        //make sure userid remains same 
        while (*pos >= 1 && 
               user_id == current->entry[*pos].user_id && 
               expense_id < current->entry[*pos].expense_id) {
            (*pos)--;
        }
        
        if (*pos >= 1 && 
            user_id == current->entry[*pos].user_id && 
            expense_id == current->entry[*pos].expense_id) {
            return true;
        }
        
        (*pos)++;
        return false;
    }
    
    (*pos)++;
    return false;
}
TreenodeExpense *InsertTreeExpense(ExpenseNode newentry, TreenodeExpense* root) {
    ExpenseNode medentry;
    TreenodeExpense* medright;
    TreenodeExpense* newroot;
    if (PushDownExpense(newentry, root, &medentry, &medright)) {
        newroot = (TreenodeExpense*)malloc(sizeof(TreenodeExpense));
        newroot->count = 1;
        newroot->entry[1] = medentry;
        newroot->branch[0] = root;
        newroot->branch[1] = medright;
        return newroot;
    }

    return root;
}
bool PushDownExpense(ExpenseNode newentry, TreenodeExpense* current, ExpenseNode* medentry, TreenodeExpense** medright) {
    int pos;
    
    if (current == NULL) {
        *medentry = newentry;
        *medright = NULL;
        return true;
    }
    
    // Find the position to insert
    SearchNodeExpense(newentry.user_id, newentry.expense_id, current, &pos);
    
    if (PushDownExpense(newentry, current->branch[pos-1], medentry, medright)) {
        if (current->count < MAX) {
            // Shift entries to make space
            for (int i = current->count; i >= pos; i--) {
                current->entry[i+1] = current->entry[i];
                current->branch[i+1] = current->branch[i];
            }
            current->entry[pos] = *medentry;
            current->branch[pos] = *medright;
            current->count++;
            return false;
        }
        else {
            // Split the node
            SplitExpense(*medentry, *medright, current, pos, medentry, medright);
            return true;
        }
    }
    return false;
}
void PushInExpense(ExpenseNode medentry, TreenodeExpense* medright, TreenodeExpense* current, int pos) {
    int i;

    // Shift entries to make space
    for (i = current->count; i >= pos; i--) {
    current->entry[i+1] = current->entry[i];
    current->branch[i+1] = current->branch[i];
    }

    // Insert new entry
    current->entry[pos] = medentry;
    current->branch[pos] = medright;
    current->count++;
}
void SplitExpense(ExpenseNode medentry, TreenodeExpense* medright, TreenodeExpense* current, int pos, ExpenseNode* newmedian, TreenodeExpense** newright) {
    int i;
    int median = (pos < MIN) ? MIN : MIN + 1;
    *newright = (TreenodeExpense*)malloc(sizeof(TreenodeExpense));
    for (i = median + 1; i <= MAX; i++) {
         (*newright)->entry[i - median] = current->entry[i];
         (*newright)->branch[i - median] = current->branch[i];
     }
     (*newright)->count = MAX - median;
     current->count = median;
     if (pos <= MIN)
         PushInExpense(medentry, medright, current, pos);
     else
         PushInExpense(medentry, medright, *newright, pos - median);
     *newmedian = current->entry[current->count];
     (*newright)->branch[0] = current->branch[current->count];
     current->count--;
}

//search and insert in tree
TreenodeFamily *SearchTreeFamily(int target, TreenodeFamily *root, int *targetpos) {
    if (!root) 
        return NULL;  

    if (SearchNodeFamily(target, root, targetpos))  
        return root;  

    return SearchTreeFamily(target, root->branch[*targetpos], targetpos);  
}
bool SearchNodeFamily(int target, TreenodeFamily *current, int *pos) {
    if (target < current->entry[1].family_id) {  
        *pos = 0;
        return false;
    }
    else {
        for (*pos = current->count; *pos >= 1 && (target < current->entry[*pos].family_id); (*pos)--);
        return (*pos >= 1 && target == current->entry[*pos].family_id);
    }
}
TreenodeFamily *InsertTreeFamily(FamilyNode newentry, TreenodeFamily* root) {
    FamilyNode medentry;
    TreenodeFamily* medright;
    TreenodeFamily* newroot;
    if (PushDownFamily(newentry, root, &medentry, &medright)) {
        newroot = (TreenodeFamily*)malloc(sizeof(TreenodeFamily));
        newroot->count = 1;
        newroot->entry[1] = medentry;
        newroot->branch[0] = root;
        newroot->branch[1] = medright;
        return newroot;
    }

    return root;
}
bool PushDownFamily(FamilyNode newentry, TreenodeFamily* current, FamilyNode* medentry, TreenodeFamily** medright) {
    int pos;
    if (current == NULL) {
        *medentry = newentry;
        *medright = NULL;
        return true;
    }
    if (SearchNodeFamily(newentry.family_id, current, &pos)) {
        printf("No duplicates allowed\n");
        return false;
    }
    if (PushDownFamily(newentry, current->branch[pos], medentry, medright)) {
        if (current->count < MAX) {
            PushInFamily(*medentry, *medright, current, pos);
            return false;
        } else {
            SplitFamily(*medentry, *medright, current, pos, medentry, medright);
            return true;
        }
    }
    return false;
}
void PushInFamily(FamilyNode medentry, TreenodeFamily* medright, TreenodeFamily* current, int pos) {
    int i;
    for(i = current->count; i > pos; i--) {
        current->entry[i+1] = current->entry[i];
        current->branch[i+1] = current->branch[i];
    }
    current->entry[pos+1] = medentry;
    current->branch[pos+1] = medright;
    current->count++;
}
void SplitFamily(FamilyNode medentry, TreenodeFamily* medright, TreenodeFamily* current, int pos, FamilyNode* newmedian, TreenodeFamily** newright) {
    int i;
    int median = (pos < MIN) ? MIN : MIN + 1;
    *newright = (TreenodeFamily*)malloc(sizeof(TreenodeFamily));
    for (i = median + 1; i <= MAX; i++) {
         (*newright)->entry[i - median] = current->entry[i];
         (*newright)->branch[i - median] = current->branch[i];
     }
     (*newright)->count = MAX - median;
     current->count = median;
     if (pos <= MIN)
         PushInFamily(medentry, medright, current, pos);
     else
         PushInFamily(medentry, medright, *newright, pos - median);
     *newmedian = current->entry[current->count];
     (*newright)->branch[0] = current->branch[current->count];
     current->count--;
}

TreenodeUser *DeleteTreeUser(int target, TreenodeUser *root) 
{ 
    // If root becomes empty after deletion, replace it with its first child
    if (root->count == 0) { 
        TreenodeUser *oldroot = root; 
        root = root->branch[0]; 
        free(oldroot); 
    }
    return root; 
}
bool DeleteFromTree(int target, TreenodeUser *current) 
{
    if (!current)
        return false;
        
    int pos;
    bool found = SearchNode(target, current, &pos);
    
    if (found) {
        // Key found in this node
        if (current->branch[pos-1] == NULL) {
            // Case 1: If node is a leaf, simply remove the key
            RemoveUser(current, pos);
        } else {
            // Case 2: If node is internal, replace with successor and delete successor
            SuccessorUser(current, pos);
            if (!DeleteFromTree(current->entry[pos].user_id, current->branch[pos]))
                return false;
        }
        return true;
    } else {
        // Key not found in this node, recursively delete from appropriate child
        if (!DeleteFromTree(target, current->branch[pos]))
            return false;
    }
    // Rebalance the tree if necessary
    if (current->branch[pos] && current->branch[pos]->count < MIN)
        RestoreUser(current, pos);
        
    return true;
}
void RemoveUser(TreenodeUser *current, int pos) 
{ 
    int i; 
    for (i = pos + 1; i <= current->count; i++) { 
        current->entry[i - 1] = current->entry[i]; 
        current->branch[i - 1] = current->branch[i]; 
    } 
    current->count--; 
}
void SuccessorUser(TreenodeUser* current, int pos){
    TreenodeUser* temp = current->branch[pos]; 
    while (temp->branch[0] != NULL) { 
        temp = temp->branch[0]; 
    } 
    current->entry[pos] = temp->entry[1]; 
}
void RestoreUser(TreenodeUser* current, int pos) {
    
    if(pos == 0){
        if(current->branch[1]->count > MIN){
            MoveLeftUser(current, 1);
        }
        else 
            CombineUser(current, 1);
    } 
    else if(pos == current->count){
        if(current->branch[pos-1]->count > MIN){
            MoveRightUser(current, pos);
        }
        else 
            CombineUser(current, pos);
    } 
    else {
        if(current->branch[pos-1]->count > MIN){
            MoveRightUser(current, pos);
        }
        else if(current->branch[pos+1]->count > MIN){
            MoveLeftUser(current, pos+1);
        }
        else{
            CombineUser(current, pos);
        }
    }
}
void MoveLeftUser(TreenodeUser* current, int pos) {
    TreenodeUser* child = current->branch[pos]; 
    TreenodeUser* sibling = current->branch[pos - 1]; 
    
    child->branch[child->count + 1] = child->branch[child->count]; 
    for (int i = child->count; i >= 1; i--) { 
        child->entry[i + 1] = child->entry[i]; 
        child->branch[i] = child->branch[i - 1]; 
    } 
    child->entry[1] = current->entry[pos]; 
    child->count++; 
    current->entry[pos] = sibling->entry[sibling->count]; 
    sibling->count--; 
}
void MoveRightUser(TreenodeUser* current, int pos) {
    TreenodeUser* child = current->branch[pos]; 
    TreenodeUser* sibling = current->branch[pos + 1]; 
    
    child->entry[child->count + 1] = current->entry[pos]; 
    child->count++; 
    current->entry[pos] = sibling->entry[1]; 
    for (int i = 2; i <= sibling->count; i++) { 
        sibling->entry[i - 1] = sibling->entry[i]; 
        sibling->branch[i - 1] = sibling->branch[i]; 
    } 
    child->branch[child->count] = sibling->branch[0]; 
    sibling->count--; 
}
void CombineUser(TreenodeUser* current, int pos) {
    TreenodeUser* leftChild = current->branch[pos]; 
    TreenodeUser* rightChild = current->branch[pos + 1]; 
    
    leftChild->entry[leftChild->count + 1] = current->entry[pos]; 
    for (int i = 1; i <= rightChild->count; i++) { 
        leftChild->entry[leftChild->count + i + 1] = rightChild->entry[i]; 
        leftChild->branch[leftChild->count + i] = rightChild->branch[i]; 
    } 
    leftChild->count += rightChild->count + 1; 
    
    for (int i = pos + 1; i <= current->count; i++) { 
        current->entry[i - 1] = current->entry[i]; 
        current->branch[i - 1] = current->branch[i]; 
    } 
    current->count--; 
}
TreenodeUser *DeleteUser(int target, TreenodeUser *root) 
{
    if (!root)
        return NULL;
        
    // Try to delete the key
    if (!DeleteFromTree(target, root))
        return root; // Key not found
        
    // Check if root is now empty
    return DeleteTreeUser(target, root);
}

TreenodeFamily *DeleteTreeFamily(int target, TreenodeFamily *root) 
{ 
    // If root becomes empty after deletion, replace it with its first child
    if (root->count == 0) { 
        TreenodeFamily *oldroot = root; 
        root = root->branch[0]; 
        free(oldroot); 
    }
    return root; 
}
bool DeleteFromTreeFamily(int target, TreenodeFamily *current) 
{
    if (!current)
        return false;
        
    int pos;
    bool found = SearchNodeFamily(target, current, &pos);
    
    if (found) {
        // Key found in this node
        if (current->branch[pos-1] == NULL) {
            // Case 1: If node is a leaf, simply remove the key
            RemoveFamily(current, pos);
        } else {
            // Case 2: If node is internal, replace with successor and delete successor
            SuccessorFamily(current, pos);
            if (!DeleteFromTreeFamily(current->entry[pos].family_id, current->branch[pos]))
                return false;
        }
        return true;
    } else {
        // Key not found in this node, recursively delete from appropriate child
        if (!DeleteFromTreeFamily(target, current->branch[pos]))
            return false;
    }
    
    // Rebalance the tree if necessary
    if (current->branch[pos] && current->branch[pos]->count < MIN)
        RestoreFamily(current, pos);
        
    return true;
}
void RemoveFamily(TreenodeFamily *current, int pos) 
{ 
    int i; 
    for (i = pos + 1; i <= current->count; i++) { 
        current->entry[i - 1] = current->entry[i]; 
        current->branch[i - 1] = current->branch[i]; 
    } 
    current->count--; 
}
void SuccessorFamily(TreenodeFamily* current, int pos){
    TreenodeFamily* temp = current->branch[pos]; 
    while (temp->branch[0] != NULL) { 
        temp = temp->branch[0]; 
    } 
    current->entry[pos] = temp->entry[1]; 
}
void RestoreFamily(TreenodeFamily* current, int pos) {
    
    if(pos == 0){
        if(current->branch[1]->count > MIN){
            MoveLeftFamily(current, 1);
        }
        else 
            CombineFamily(current, 1);
    } 
    else if(pos == current->count){
        if(current->branch[pos-1]->count > MIN){
            MoveRightFamily(current, pos);
        }
        else 
            CombineFamily(current, pos);
    } 
    else {
        if(current->branch[pos-1]->count > MIN){
            MoveRightFamily(current, pos);
        }
        else if(current->branch[pos+1]->count > MIN){
            MoveLeftFamily(current, pos+1);
        }
        else{
            CombineFamily(current, pos);
        }
    }
}
void MoveLeftFamily(TreenodeFamily* current, int pos) {
    TreenodeFamily* child = current->branch[pos]; 
    TreenodeFamily* sibling = current->branch[pos - 1]; 
    
    child->branch[child->count + 1] = child->branch[child->count]; 
    for (int i = child->count; i >= 1; i--) { 
        child->entry[i + 1] = child->entry[i]; 
        child->branch[i] = child->branch[i - 1]; 
    } 
    child->entry[1] = current->entry[pos]; 
    child->count++; 
    current->entry[pos] = sibling->entry[sibling->count]; 
    sibling->count--; 
}
void MoveRightFamily(TreenodeFamily* current, int pos) {
    TreenodeFamily* child = current->branch[pos]; 
    TreenodeFamily* sibling = current->branch[pos + 1]; 
    
    child->entry[child->count + 1] = current->entry[pos]; 
    child->count++; 
    current->entry[pos] = sibling->entry[1]; 
    for (int i = 2; i <= sibling->count; i++) { 
        sibling->entry[i - 1] = sibling->entry[i]; 
        sibling->branch[i - 1] = sibling->branch[i]; 
    } 
    child->branch[child->count] = sibling->branch[0]; 
    sibling->count--; 
}
void CombineFamily(TreenodeFamily* current, int pos) {
    TreenodeFamily* leftChild = current->branch[pos]; 
    TreenodeFamily* rightChild = current->branch[pos + 1]; 
    
    leftChild->entry[leftChild->count + 1] = current->entry[pos]; 
    for (int i = 1; i <= rightChild->count; i++) { 
        leftChild->entry[leftChild->count + i + 1] = rightChild->entry[i]; 
        leftChild->branch[leftChild->count + i] = rightChild->branch[i]; 
    } 
    leftChild->count += rightChild->count + 1; 
    
    for (int i = pos + 1; i <= current->count; i++) { 
        current->entry[i - 1] = current->entry[i]; 
        current->branch[i - 1] = current->branch[i]; 
    } 
    current->count--; 
}
TreenodeFamily *DeleteFamily(int target, TreenodeFamily *root) 
{
    if (!root)
        return NULL;
        
    // Try to delete the key
    if (!DeleteFromTreeFamily(target, root))
        return root; // Key not found
        
    // Check if root is now empty
    return DeleteTreeFamily(target, root);
}

TreenodeExpense *DeleteTreeExpense(int user_id, int expense_id, TreenodeExpense *root) 
{ 
    // If root becomes empty after deletion, replace it with its first child
    if (root->count == 0) { 
        TreenodeExpense *oldroot = root; 
        root = root->branch[0]; 
        free(oldroot); 
    }
    return root; 
}
bool DeleteFromTreeExpense(int user_id, int expense_id, TreenodeExpense *current) 
{
    if (!current)
        return false;
        
    int pos;
    bool found = SearchNodeExpense(user_id, expense_id, current, &pos);
    
    if (found) {
        // Key found in this node
        if (current->branch[pos-1] == NULL) {
            // Case 1: If node is a leaf, simply remove the key
            RemoveExpense(current, pos);
        } else {
            // Case 2: If node is internal, replace with successor and delete successor
            SuccessorExpense(current, pos);
            // Now we need to delete the successor
            // The tricky part is we need to use the replacement's IDs for recursive deletion
            if (!DeleteFromTreeExpense(current->entry[pos].user_id, 
                                     current->entry[pos].expense_id, 
                                     current->branch[pos]))
                return false;
        }
        return true;
    } else {
        // Key not found in this node, recursively delete from appropriate child
        if (!DeleteFromTreeExpense(user_id, expense_id, current->branch[pos]))
            return false;
    }
    
    // Rebalance the tree if necessary
    if (current->branch[pos] && current->branch[pos]->count < MIN)
        RestoreExpense(current, pos);
        
    return true;
}
void RemoveExpense(TreenodeExpense *current, int pos) 
{ 
    int i; 
    for (i = pos + 1; i <= current->count; i++) { 
        current->entry[i - 1] = current->entry[i]; 
        current->branch[i - 1] = current->branch[i]; 
    } 
    current->count--; 
}
void SuccessorExpense(TreenodeExpense* current, int pos){
    TreenodeExpense* temp = current->branch[pos]; 
    while (temp->branch[0] != NULL) { 
        temp = temp->branch[0]; 
    } 
    current->entry[pos] = temp->entry[1]; 
}
void RestoreExpense(TreenodeExpense* current, int pos) {
    
    if(pos == 0){
        if(current->branch[1]->count > MIN){
            MoveLeftExpense(current, 1);
        }
        else 
            CombineExpense(current, 1);
    } 
    else if(pos == current->count){
        if(current->branch[pos-1]->count > MIN){
            MoveRightExpense(current, pos);
        }
        else 
            CombineExpense(current, pos);
    } 
    else {
        if(current->branch[pos-1]->count > MIN){
            MoveRightExpense(current, pos);
        }
        else if(current->branch[pos+1]->count > MIN){
            MoveLeftExpense(current, pos+1);
        }
        else{
            CombineExpense(current, pos);
        }
    }
}
void MoveLeftExpense(TreenodeExpense* current, int pos) {
    TreenodeExpense* child = current->branch[pos]; 
    TreenodeExpense* sibling = current->branch[pos - 1]; 
    
    child->branch[child->count + 1] = child->branch[child->count]; 
    for (int i = child->count; i >= 1; i--) { 
        child->entry[i + 1] = child->entry[i]; 
        child->branch[i] = child->branch[i - 1]; 
    } 
    child->entry[1] = current->entry[pos]; 
    child->count++; 
    current->entry[pos] = sibling->entry[sibling->count]; 
    sibling->count--; 
}
void MoveRightExpense(TreenodeExpense* current, int pos) {
    TreenodeExpense* child = current->branch[pos]; 
    TreenodeExpense* sibling = current->branch[pos + 1]; 
    
    child->entry[child->count + 1] = current->entry[pos]; 
    child->count++; 
    current->entry[pos] = sibling->entry[1]; 
    for (int i = 2; i <= sibling->count; i++) { 
        sibling->entry[i - 1] = sibling->entry[i]; 
        sibling->branch[i - 1] = sibling->branch[i]; 
    } 
    child->branch[child->count] = sibling->branch[0]; 
    sibling->count--; 
}
void CombineExpense(TreenodeExpense* current, int pos) {
    TreenodeExpense* leftChild = current->branch[pos]; 
    TreenodeExpense* rightChild = current->branch[pos + 1]; 
    
    leftChild->entry[leftChild->count + 1] = current->entry[pos]; 
    for (int i = 1; i <= rightChild->count; i++) { 
        leftChild->entry[leftChild->count + i + 1] = rightChild->entry[i]; 
        leftChild->branch[leftChild->count + i] = rightChild->branch[i]; 
    } 
    leftChild->count += rightChild->count + 1; 
    
    for (int i = pos + 1; i <= current->count; i++) { 
        current->entry[i - 1] = current->entry[i]; 
        current->branch[i - 1] = current->branch[i]; 
    } 
    current->count--; 
    
    free(rightChild);
}
// Main entry point for expense deletion
TreenodeExpense *DeleteExpense(int user_id, int expense_id, TreenodeExpense *root) 
{
    if (!root)
        return NULL;
        
    // Try to delete the expense
    if (!DeleteFromTreeExpense(user_id, expense_id, root))
        return root; // Expense not found
        
    // Check if root is now empty
    return DeleteTreeExpense(user_id, expense_id, root);
}

void LoadUsers(TreenodeUser** userRoot, FILE* file) {
    char line[256];
    int user_id;
    char user_name[NAME];
    float income;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0 || strcmp(line, "Families") == 0) {
            break;
        }
        if (sscanf(line, "%d %s %f", &user_id, user_name, &income) == 3) {
            UserNode newUser = CreateUserNode(user_id, user_name, income);
            *userRoot = InsertTree(newUser, *userRoot);
        }
    }
}
void LoadFamilies(TreenodeFamily** familyRoot, FILE* file) {
    char line[256];
    int family_id;
    char family_name[NAME];
    float total_income, total_expense;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0 || strcmp(line, "Memberships") == 0) {
            break;
        }
        if (sscanf(line, "%d %s %f %f", &family_id, family_name, &total_income, &total_expense) == 4) {
            FamilyNode newFamily = CreateFamilyNode(family_id, family_name, total_income, total_expense);
            *familyRoot = InsertTreeFamily(newFamily, *familyRoot);
        }
    }
}
void LoadMemberships(TreenodeUser** userRoot, TreenodeFamily** familyRoot, FILE* file) {
    char line[256];
    int user_id, family_id;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        
        if (strlen(line) == 0 || strcmp(line, "Expenses") == 0) {
            break;
        }
        if (sscanf(line, "%d %d", &user_id, &family_id) == 2) {
            int userPos = 0;
            TreenodeUser* userNodeTree = SearchTree(user_id, *userRoot, &userPos);
            
            int familyPos = 0;
            TreenodeFamily* familyNodeTree = SearchTreeFamily(family_id, *familyRoot, &familyPos);
            
            if (userNodeTree && familyNodeTree && 
                userPos > 0 && userPos <= userNodeTree->count &&
                familyPos > 0 && familyPos <= familyNodeTree->count) {
                
                UserNode* user = &userNodeTree->entry[userPos];
                FamilyNode* family = &familyNodeTree->entry[familyPos];
                if (user->user_id == user_id && family->family_id == family_id) {
                   
                    if (family->member_count < FAMILY_MEMBERS) {
                        user->family = family;
                        family->member[family->member_count++] = user;
                    }
                }
            }
        }
    }
}
void LoadExpenses(TreenodeExpense** expenseRoot, TreenodeUser** userRoot, FILE* file) {
    char line[256];
    int expense_id, user_id, category, date, month;
    float amount;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) {
            continue;
        }
        if (sscanf(line, "%d %d %f %d %d %d", &expense_id, &user_id, &amount, &category, &date, &month) == 6) {
            
            ExpenseNode newExpense = CreateExpenseNode(expense_id, user_id, amount, category, date, month);
            *expenseRoot = InsertTreeExpense(newExpense, *expenseRoot);
            int userPos = 0;
            TreenodeUser* userNodeTree = SearchTree(user_id, *userRoot, &userPos);
            
            if (userNodeTree && userPos > 0 && userPos <= userNodeTree->count) {
                UserNode* user = &userNodeTree->entry[userPos];
                if (user->user_id == user_id && user->expense_count < 10) {
                   
                    int i = user->expense_count;
                    while (i > 0 && user->exp[i-1] > expense_id) {
                        user->exp[i] = user->exp[i-1];
                        i--;
                    }
                    user->exp[i] = expense_id;
                    user->expense_count++;
                }
            }
        }
    }
}

// Helper functions to save each data type to the file
void SaveUsers(TreenodeUser* root, FILE* file) {
    if (root == NULL) {
        return;
    }
    int i;
    SaveUsers(root->branch[0], file);
    for (i = 1; i <= root->count; i++) {
        fprintf(file, "%d %s %.2f\n", 
               root->entry[i].user_id, 
               root->entry[i].user_name, 
               root->entry[i].income);
        SaveUsers(root->branch[i], file);
    }
}
void SaveFamilies(TreenodeFamily* root, FILE* file) {
    if (root == NULL) {
        return;
    }
    int i;
    SaveFamilies(root->branch[0], file);
    for (i = 1; i <= root->count; i++) {
        fprintf(file, "%d %s %.2f %.2f\n", 
               root->entry[i].family_id, 
               root->entry[i].name, 
               root->entry[i].total_income, 
               root->entry[i].total_expense);
        SaveFamilies(root->branch[i], file);
    }
}
void SaveMemberships(TreenodeUser* root, FILE* file) {
    if (root == NULL) {
        return;
    }
    int i;
    SaveMemberships(root->branch[0], file);
    for (i = 1; i <= root->count; i++) {
        if (root->entry[i].family != NULL) {
            fprintf(file, "%d %d\n", 
                  root->entry[i].user_id, 
                  root->entry[i].family->family_id);
        }
        SaveMemberships(root->branch[i], file);
    }
}
void SaveExpenses(TreenodeExpense* root, FILE* file) {
    if (root == NULL) {
        return;
    }
    int i;
    SaveExpenses(root->branch[0], file);
    for (i = 1; i <= root->count; i++) {
        fprintf(file, "%d %d %.2f %d %d %d\n", 
               root->entry[i].expense_id, 
               root->entry[i].user_id, 
               root->entry[i].amount, 
               root->entry[i].category, 
               root->entry[i].date.date, 
               root->entry[i].date.month);
        SaveExpenses(root->branch[i], file);
    }
}

void LoadAllData(TreenodeUser** userRoot, TreenodeFamily** familyRoot, TreenodeExpense** expenseRoot) {
    *userRoot = NULL;
    *familyRoot = NULL;
    *expenseRoot = NULL;
    
    FILE* dataFile = fopen("new.txt", "r");
    if (!dataFile) {
        printf("Note: data.txt not found, starting with an empty database\n");
        return;
    }
    char line[256];
    rewind(dataFile);
    
    while (fgets(line, sizeof(line), dataFile)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, "Users") == 0) {
            LoadUsers(userRoot, dataFile);
        } else if (strcmp(line, "Families") == 0) {
            LoadFamilies(familyRoot, dataFile);
        } else if (strcmp(line, "Memberships") == 0) {
            LoadMemberships(userRoot, familyRoot, dataFile);
        } else if (strcmp(line, "Expenses") == 0) {
            LoadExpenses(expenseRoot, userRoot, dataFile);
        }
    }
    
    fclose(dataFile);
    printf("Data loaded successfully from new.txt\n");
}
void SaveAllData(TreenodeUser* userRoot, TreenodeFamily* familyRoot, TreenodeExpense* expenseRoot) {
    FILE* dataFile = fopen("new.txt", "w");
    if (!dataFile) {
        printf("Error: Could not open data.txt for writing\n");
        return;
    }
    fprintf(dataFile, "Users\n");
    SaveUsers(userRoot, dataFile);
    fprintf(dataFile, "\nFamilies\n");
    SaveFamilies(familyRoot, dataFile);
    fprintf(dataFile, "\nMemberships\n");
    SaveMemberships(userRoot, dataFile);
    fprintf(dataFile, "\nExpenses\n");
    SaveExpenses(expenseRoot, dataFile);
    fclose(dataFile);
    printf("All data saved to data.txt successfully\n");
}

int AddUser(TreenodeUser** root, TreenodeFamily** familyRoot, TreenodeExpense* expenseRoot) {
    int user_id;
    char user_name[NAME];
    float income;
    
    printf("Enter User ID: ");
    scanf("%d", &user_id);
    printf("Enter Name: ");
    scanf("%s", user_name);
    printf("Enter Income: ");
    scanf("%f", &income);
    UserNode newUser = CreateUserNode(user_id, user_name, income);
    
    printf("Select 1 to create family and 2 to join family: ");
    int family_choice;
    if (scanf("%d", &family_choice) != 1 || (family_choice != 1 && family_choice != 2)) {
        printf("Invalid choice. User not added.\n");
        while (getchar() != '\n'); 
        return 0;
    }
    
    int family_id;
    printf("Enter Family ID: ");
    scanf("%d", &family_id);
    *root = InsertTree(newUser, *root);
    bool family_success = false;
    
    if (family_choice == 1) {
        printf("Enter Family Name: ");
        char family_name[NAME];
        scanf("%s", family_name);
        family_success = CreateFamily(familyRoot, *root, user_id, family_id, family_name, expenseRoot);
        
        if (family_success) {
            printf("User added and Family created successfully\n");
        } else {
            printf("Error: User added but could not create family.\n");
        }
    } else if (family_choice == 2) {
        int familyPos = 0;
        TreenodeFamily* familyNode = SearchTreeFamily(family_id, *familyRoot, &familyPos);
        
        if (familyNode != NULL && familyPos >= 1 && 
            familyPos <= familyNode->count && 
            familyNode->entry[familyPos].family_id == family_id) {
            
            JoinFamily(*familyRoot, *root, user_id, family_id, expenseRoot);
            family_success = true;
            printf("User added and joined Family successfully\n");
        } else {
            printf("Error: Family with ID %d not found. User added but not assigned to a family.\n", family_id);
        }
    }
    if (!family_success) {
        int userPos = 0;
        TreenodeUser* userNode = SearchTree(user_id, *root, &userPos);
        
        if (userNode != NULL && userPos >= 1 && 
            userPos <= userNode->count && 
            userNode->entry[userPos].user_id == user_id) {
            
            *root = DeleteUser(user_id, *root);
            printf("User removed from tree due to family assignment failure.\n");
        } else {
            printf("Error: User with ID %d not found in tree.\n", user_id);
        }
    }
    return 1;
}

bool CreateFamily(TreenodeFamily** familyRoot, TreenodeUser* root, int user_id, int family_id, char* name, TreenodeExpense* expenseRoot) {
    // Search for the user in the user tree
    int userPos = 0;
    TreenodeUser* userNode = SearchTree(user_id, root, &userPos);
    if (userNode == NULL) {
        printf("Error: User with ID %d not found.\n", user_id);
        return false;
    }
    if (userPos < 1 || userPos > userNode->count || userNode->entry[userPos].user_id != user_id) {
        printf("Error: User with ID %d not found in node.\n", user_id);
        return false;
    }
    UserNode* user = &userNode->entry[userPos];
    
    float total_expense = 0;
    for (int j = 0; j < user->expense_count; j++) {
    int expense_id = user->exp[j];  // Get the expense ID from user's exp array
    int expensePos;
    
    // Search for this expense in the expense B-tree
    TreenodeExpense* expenseNode = SearchTreeExpense(user->user_id ,expense_id, expenseRoot, &expensePos);
    
    // If found, add its amount to the total
    if (expenseNode != NULL && expensePos >= 1) {
        total_expense += expenseNode->entry[expensePos].amount;
    }
}
    FamilyNode newFamily;
    newFamily = CreateFamilyNode(family_id, name, user->income, total_expense);
    newFamily.member[0] = user; 
    newFamily.member_count = 1; 
    
    *familyRoot = InsertTreeFamily(newFamily, *familyRoot);
    
    user->family = &(*familyRoot)->entry[1]; 
    
    printf("Family with ID %d created successfully.\n", family_id);
    return true;
}

void JoinFamily(TreenodeFamily* familyRoot, TreenodeUser* root, int user_id, int family_id, TreenodeExpense* expenseRoot) {
    // Search for the user
    int userPos = 0;
    TreenodeUser* userNode = SearchTree(user_id, root, &userPos);
    if (userNode == NULL || userPos < 1 || userPos > userNode->count || userNode->entry[userPos].user_id != user_id) {
        printf("Error: User with ID %d not found.\n", user_id);
        return;
    }
    
    int familyPos = 0;
    TreenodeFamily* familyNode = SearchTreeFamily(family_id, familyRoot, &familyPos);
    if (familyNode == NULL || familyPos < 1 || familyPos > familyNode->count || familyNode->entry[familyPos].family_id != family_id) {
        printf("Error: Family with ID %d not found.\n", family_id);
        return;
    }
    UserNode* user = &userNode->entry[userPos];
    FamilyNode* family = &familyNode->entry[familyPos];
    
    float total_expense = 0;
    for (int j = 0; j < user->expense_count; j++) {
    int expense_id = user->exp[j];  // Get the expense ID from user's exp array
    int expensePos;
    
    TreenodeExpense* expenseNode = SearchTreeExpense(user->user_id,expense_id, expenseRoot, &expensePos);

    if (expenseNode != NULL && expensePos >= 1) {
        total_expense += expenseNode->entry[expensePos].amount;
    }
}

    for (int k = 0; k < family->member_count; k++) {
        if (family->member[k] != NULL && family->member[k]->user_id == user_id) {
            printf("User with ID %d is already a member of Family with ID %d\n", user_id, family_id);
            return;
        }
    }
    if (family->member_count >= FAMILY_MEMBERS) {
        printf("Error: Family has reached maximum member capacity.\n");
        return;
    }

    family->member[family->member_count] = user;
    family->total_income += user->income;
    family->total_expense += total_expense;
    family->member_count++;
    
    user->family = family;
    
    printf("User with ID %d successfully joined Family with ID %d\n", user_id, family_id);
}

void addExpense(TreenodeUser* userRoot, TreenodeExpense** expenseRoot) {
    int expense_id, user_id;
    float amount;
    ExpenseCategory category;
    day_and_date date;
    
    printf("Enter Expense ID: ");
    scanf("%d", &expense_id);
    
    printf("Enter User ID: ");
    scanf("%d", &user_id);
    
    // Check if user exists first
    int targetPos;
    TreenodeUser* userNodeTree = SearchTree(user_id, userRoot, &targetPos);
    if (userNodeTree == NULL) {
        printf("Error: User not found\n");
        return ;
    }
    
    // Get the UserNode
    UserNode* userNode = &userNodeTree->entry[targetPos];
    
    // Check if expense with same user_id and expense_id already exists
    if (SearchTreeExpense(user_id, expense_id, *expenseRoot, &targetPos) != NULL) {
        printf("Error: Expense with this ID already exists for this user\n");
        return ;
    }
    
    // Check if user has reached maximum expense limit
    if (userNode->expense_count >= 10) {
        printf("Error: User has reached maximum expense limit\n");
        return ;
    }
    
    printf("Enter Amount: ");
    scanf("%f", &amount);
    
    printf("Enter Category (0-Rent, 1-Grocery, 2-Utility, 3-Leisure, 4-Stationery, 5-Others): ");
    scanf("%d", (int*)&category);
    
    // Validate category
    if (category < 0 || category > 5) {
        printf("Error: Invalid category\n");
        return ;
    }
    
    printf("Enter date (date month): ");
    scanf("%d %d", &date.date, &date.month);
    
    // Validate date
    if (date.date < 1 || date.date > 31 || date.month < 1 || date.month > 12) {
        printf("Error: Invalid date\n");
        return ;
    }
    
    ExpenseNode newExpense = CreateExpenseNode(expense_id, user_id, amount, category, date.date, date.month);
    
    // Verify family pointer is valid before updating
    if (userNode->family == NULL) {
        printf("Error: User has no family information\n");
        return ;
    }
    
    // Update family total expense
    userNode->family->total_expense += amount;
    
    // Add the expense ID to the user's exp array (sorted)
    int i = userNode->expense_count;
    while (i > 0 && userNode->exp[i-1] > expense_id) {
        userNode->exp[i] = userNode->exp[i-1];
        i--;
    }
    userNode->exp[i] = expense_id;
    userNode->expense_count++;
    
    // Insert the expense into the expense tree
    *expenseRoot = InsertTreeExpense(newExpense, *expenseRoot);
    printf("Expense added successfully!\n");
    return ;
}
    
void GetIndividualExpense(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int user_id) {
    UserNode* userNode = GetUserById(userRoot, user_id);
    if (userNode == NULL) {
        return;
    }
    float total_expense = 0;
    float category_totals[6] = {0}; 
    char* category_names[6] = {"Rent", "Grocery", "Utility", "Leisure", "Stationery", "Others"};

    for (int i = 0; i < userNode->expense_count; i++) {
        int expense_id = userNode->exp[i];
        int expensePos;
        TreenodeExpense* expenseNode = SearchTreeExpense(userNode->user_id,expense_id, expenseRoot, &expensePos);
        
        if (expenseNode != NULL && expensePos >= 1) {
            ExpenseNode* expense = &expenseNode->entry[expensePos];
            total_expense += expense->amount;
            category_totals[expense->category] += expense->amount;
        }
    }

    
    int indices[6] = {0, 1, 2, 3, 4, 5};
    
    // Bubble sort the indices based on category totals
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5 - i; j++) {
            if (category_totals[indices[j]] < category_totals[indices[j + 1]]) {
                // Swap indices
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }
    printf("\nExpense Summary for User ID %d:\n", user_id);
    printf("Total Expenses: %.2f\n\n", total_expense);
    
    for (int i = 0; i < 6; i++) {
        int category = indices[i];
        if (category_totals[category] > 0) {
            printf("%-10s: %.2f \n", 
                   category_names[category], 
                   category_totals[category]);
        }
    }
}

int GetTotalExpenseIndividual(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int user_id, int month) {
    float total_expense = 0;
    UserNode* userNode = GetUserById(userRoot, user_id);
    
    if (userNode == NULL) {
        printf("Error: User with ID %d not found in node.\n", user_id);
        return 0;
    }
    for (int j = 0; j < userNode->expense_count; j++) {
        int expense_id = userNode->exp[j];
        int expensePos;
        TreenodeExpense* expenseNode = SearchTreeExpense(userNode->user_id,expense_id, expenseRoot, &expensePos);
        
        if (expenseNode != NULL && expensePos >= 1) {
            ExpenseNode* expense = &expenseNode->entry[expensePos];
            if (expense->date.month == month) {
                total_expense += expense->amount;
            }
        }
    }
    return total_expense;
}

void GetMonthlyExpense(TreenodeFamily* familyRoot, TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int family_id) {
    int month;
    printf("Enter month (1-12): ");
    scanf("%d", &month);
    if (month < 1 || month > 12) {
        printf("Error: Invalid month. Please enter a number between 1 and 12.\n");
        return;
    }
    int targetPos;
    TreenodeFamily* familyNode = SearchTreeFamily(family_id, familyRoot, &targetPos);
    if (familyNode == NULL) {
        printf("Error: Family with ID %d not found.\n", family_id);
        return;
    }
    FamilyNode* family = NULL;
    for (int i = 1; i <= familyNode->count; i++) {
        if (familyNode->entry[i].family_id == family_id) {
            family = &familyNode->entry[i];
            break;
        }
    }

    if (family == NULL) {
        printf("Error: Family with ID %d not found in node.\n", family_id);
        return;
    }
    float total_expense = 0;
    for (int i = 0; i < family->member_count; i++) {
        total_expense += GetTotalExpenseIndividual(userRoot, expenseRoot, family->member[i]->user_id, month);
    }

    printf("\n=== Family Expense Report for Month %d ===\n", month);
    printf("Family ID: %d\n", family_id);
    printf("Total Income: %.2f\n", family->total_income);
    printf("Total Expenses: %.2f\n", total_expense);
    
    float difference = family->total_income - total_expense;
    if (difference >= 0) {
        printf("Status: Within budget (Income exceeds expenses by %.2f)\n", difference);
    } else {
        printf("Status: Over budget (Expenses exceed income by %.2f)\n", -difference);
    }
}

void GetHighestExpenseDay(TreenodeFamily* familyRoot, TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int family_id){
    int targetPos;
    TreenodeFamily* familyNode = SearchTreeFamily(family_id, familyRoot, &targetPos);
    if (familyNode == NULL) {
        printf("Error: Family with ID %d not found.\n", family_id);
        return;
    }
    FamilyNode* family = NULL;
    for (int i = 1; i <= familyNode->count; i++) {
        if (familyNode->entry[i].family_id == family_id) {
            family = &familyNode->entry[i];
            break;
        }
    }

    if (family == NULL) {
        printf("Error: Family with ID %d not found in node.\n", family_id);
        return;
    }
    int day = 0;
    int month = 0;
    float max = 0;
    for(int i = 0;i<family->member_count;i++){
        UserNode* user = family->member[i];
        for(int j = 0;j<user->expense_count;j++){
            int expense_id = user->exp[j];
            int expensePos;
            TreenodeExpense* expenseNode = SearchTreeExpense(user->user_id,expense_id, expenseRoot, &expensePos);
            
            if (expenseNode != NULL && expensePos >= 1) {
                ExpenseNode* expense = &expenseNode->entry[expensePos];
                if(expense->amount > max){
                    max = expense->amount;
                    day = expense->date.date;
                    month = expense->date.month;
                }
            }
        }
    }
    if(max == 0){
        printf("No expenses found for family ID %d\n", family_id);
        return;
    }
    else{
        printf("Highest expense for family ID %d is %.2f on %d/%d\n", family_id, max, day, month);
    }
}

void GetExpenseInPeriod(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int start_day, int start_month, int end_day, int end_month) {
    if (userRoot == NULL) {
        return;
    }
    GetExpenseInPeriod(userRoot->branch[0], expenseRoot, start_day, start_month, end_day, end_month);

    for (int i = 1; i <= userRoot->count; i++) {
        UserNode* user = &userRoot->entry[i];
            for (int j = 0; j < user->expense_count; j++) {
                int expense_id = user->exp[j];
                int expensePos;
                TreenodeExpense* expenseNode = SearchTreeExpense(user->user_id, expense_id, expenseRoot, &expensePos);
                
                if (expenseNode != NULL && expensePos >= 1) {
                    ExpenseNode* expense = &expenseNode->entry[expensePos];
                    if ((expense->date.month > start_month || 
                        (expense->date.month == start_month && expense->date.date >= start_day)) &&
                       (expense->date.month < end_month || 
                        (expense->date.month == end_month && expense->date.date <= end_day))){
                            printf("User ID: %d, Expense ID: %d, Amount: %.2f, Category: %d, Date: %d/%d\n",
                                   user->user_id, expense->expense_id, expense->amount, expense->category,
                                   expense->date.date, expense->date.month);
                        }
                    }
                } 
            }
    for (int i = 1; i <= userRoot->count; i++) {
        GetExpenseInPeriod(userRoot->branch[i], expenseRoot, start_day, start_month, end_day, end_month);
    }
}

void GetExpenseInRange(TreenodeUser* userRoot, int exp1, int exp2, int user_id, TreenodeExpense* expenseRoot) {
    if (userRoot == NULL || expenseRoot == NULL) {
        printf("Error: Invalid tree structures.\n");
        return;
    }
    int userPos = 0;
    TreenodeUser* userNodeTree = SearchTree(user_id, userRoot, &userPos);
    
    if (userNodeTree == NULL || userPos < 1 || userPos > userNodeTree->count || 
        userNodeTree->entry[userPos].user_id != user_id) {
        printf("Error: User with ID %d not found.\n", user_id);
        return;
    }
    UserNode* userNode = &userNodeTree->entry[userPos];
    
    if (userNode->expense_count == 0) {
        printf("User ID %d has no expenses.\n", user_id);
        return;
    }
    
    printf("Expenses for User ID %d in range %d to %d:\n", user_id, exp1, exp2);
    
    int foundCount = 0;
    
    for (int i = 0; i < userNode->expense_count; i++) {
        int expense_id = userNode->exp[i];
        if (expense_id >= exp1 && expense_id <= exp2) {
            int expensePos = 0;
            
            TreenodeExpense* expenseNode = SearchTreeExpense(user_id, expense_id, expenseRoot, &expensePos);
       
            if (expenseNode != NULL && expensePos >= 1 && expensePos <= expenseNode->count &&
                expenseNode->entry[expensePos].expense_id == expense_id) {
                
                ExpenseNode* expense = &expenseNode->entry[expensePos];
                printf("  Expense ID: %d, Amount: %.2f, Category: ", 
                       expense_id, expense->amount);
                
                // Print category name
                switch(expense->category) {
                    case Rent: printf("Rent"); break;
                    case Grocery: printf("Grocery"); break;
                    case Utility: printf("Utility"); break;
                    case Leisure: printf("Leisure"); break;
                    case Stationary: printf("Stationary"); break;
                    default: printf("Others");
                }
                
                printf(", Date: %d/%d\n", expense->date.date, expense->date.month);
                foundCount++;
            } else {
                printf("  Warning: Expense ID %d referenced by user but not found in expense tree.\n", expense_id);
            }
        }
    }
    
    if (foundCount == 0) {
        printf("No expenses found in the specified range.\n");
    } else {
        printf("Total expenses found in range: %d\n", foundCount);
    }
}

float GetIndividualExpenseCategory(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, int user_id, int category) {
    float total_expense = 0;
    UserNode* userNode = GetUserById(userRoot, user_id);
    
    if (userNode == NULL) {
        printf("Error: User with ID %d not found in node.\n", user_id);
        return 0;
    }
    for (int j = 0; j < userNode->expense_count; j++) {
        int expense_id = userNode->exp[j];
        int expensePos;
        TreenodeExpense* expenseNode = SearchTreeExpense(userNode->user_id,expense_id, expenseRoot, &expensePos);
        
        if (expenseNode != NULL && expensePos >= 1) {
            ExpenseNode* expense = &expenseNode->entry[expensePos];
            if (expense->category == category) {
                total_expense += expense->amount;
            }
        }
    }
    return total_expense;
}

void GetCategoricalExpense(TreenodeUser* userRoot, TreenodeExpense* expenseRoot, TreenodeFamily* familyRoot, int family_id, int category){
    int targetPos;
    TreenodeFamily* familyNode = SearchTreeFamily(family_id, familyRoot, &targetPos);
    if (familyNode == NULL) {
        printf("Error: Family with ID %d not found.\n", family_id);
        return;
    }
    FamilyNode* family = NULL;
    for (int i = 1; i <= familyNode->count; i++) {
        if (familyNode->entry[i].family_id == family_id) {
            family = &familyNode->entry[i];
            break;
        }
    }

    if (family == NULL) {
        printf("Error: Family with ID %d not found in node.\n", family_id);
        return;
    }
    float arr[family->member_count];
    int ar1[family->member_count];
    for(int i = 0;i<family->member_count;i++){
        ar1[i] = family->member[i]->user_id;
    }
    float total_expense = 0;
    for (int i = 0; i < family->member_count; i++) {
        float exp = GetIndividualExpenseCategory(userRoot, expenseRoot, family->member[i]->user_id, category);
        arr[i] = exp;
        total_expense += exp;
    }
    for(int i = 0;i<family->member_count;i++){
        for(int j = 0;j<family->member_count-i-1;j++){
            if(arr[j] < arr[j+1]){
                float temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                int temp1 = ar1[j];
                ar1[j] = ar1[j+1];
                ar1[j+1] = temp1;
            }
        }
    }
    printf("\n=== Family Categorical Expense Report for Category %d ===\n", category);
    printf("Family ID: %d\n", family_id);
    printf("Total Categorical Expenses: %.2f\n", total_expense);
    for(int i = 0;i<family->member_count;i++){
        printf("User ID: %d, Categorical Expense: %.2f\n", ar1[i], arr[i]);
    }
}

void deleteUser(TreenodeUser* userRoot, int user_id, TreenodeExpense* expenseRoot, TreenodeFamily* familyRoot) {
    UserNode* userNode = GetUserById(userRoot, user_id);
    if (userNode == NULL) {
        printf("Error: User with ID %d not found in node.\n", user_id);
        return ;
    }
    if(userNode->family->member_count == 1){
        DeleteFamily(userNode->family->family_id, familyRoot);
    } 
    if (userNode->family != NULL) {
        
        FamilyNode* familyNode = userNode->family;
        for (int i = 0; i < familyNode->member_count; i++) {
            if (familyNode->member[i] == userNode) {
                
                for (int j = i; j < familyNode->member_count - 1; j++) {
                    familyNode->member[j] = familyNode->member[j + 1];
                }
                familyNode->member[familyNode->member_count - 1] = NULL;
                familyNode->member_count--;
                break;
            }
        }
        familyNode->total_income -= userNode->income;
    
        userNode->family = NULL; // Clear family reference in user node
        for(int i = 0;i<userNode->expense_count;i++){
            int expense_id = userNode->exp[i];
            int expensePos;
            TreenodeExpense* expenseNode = SearchTreeExpense(userNode->user_id,expense_id, expenseRoot, &expensePos);
            
            if (expenseNode != NULL && expensePos >= 1) {
                ExpenseNode* expense = &expenseNode->entry[expensePos];
                familyNode->total_expense -= expense->amount;
            }
            // implement delete expense tree se bhi
            for(int i = 0;i<userNode->expense_count;i++){
                int expense_id = userNode->exp[i];
                DeleteExpense(user_id, expense_id, expenseRoot);
                userNode->exp[i] = 0;
            }
        }
    DeleteUser(user_id, userRoot);
    }
}
void deleteFamily(TreenodeFamily* familyRoot, int family_id, TreenodeExpense* expenseRoot) {
    // Search for the family
    int targetPos;
    TreenodeFamily* familyNode = SearchTreeFamily(family_id, familyRoot, &targetPos);
    
    if (familyNode == NULL) {
        printf("Error: Family with ID %d not found.\n", family_id);
        return;
    }
    
    FamilyNode* family = NULL;
    for (int i = 1; i <= familyNode->count; i++) {
        if (familyNode->entry[i].family_id == family_id) {
            family = &familyNode->entry[i];
            break;
        }
    }
    
    if (family == NULL) {
        printf("Error: Family with ID %d not found in node.\n", family_id);
        return;
    }
    int memberCount = family->member_count; 
    for (int i = 0; i < memberCount; i++) {
        UserNode* user = family->member[i];
        if (user == NULL) continue;
        user->family = NULL;
        
        for (int k = 0; k < user->expense_count; k++) {
            int expense_id = user->exp[k];
            DeleteExpense(user->user_id, expense_id, expenseRoot);
            user->exp[k] = 0; // Clear the expense ID from the user's array
        }
        user->expense_count = 0;
    }
    for (int i = 0; i < family->member_count; i++) {
        family->member[i] = NULL;
    }
    family->member_count = 0;
    family->total_income = 0;
    family->total_expense = 0;
    
    DeleteFamily(family_id, familyRoot);
}
void UpdateUser(TreenodeUser* root, int user_id) {
    UserNode* userNode = GetUserById(root, user_id);
    if (userNode == NULL) {
        printf("Error: User with ID %d not found in node.\n", user_id);
        return ;
    }
    printf("Enter new income\n");
    float new_income;
    scanf("%f", &new_income);
    userNode->family->total_income = userNode->family->total_income - userNode->income + new_income;
    userNode->income = new_income;
    printf("User details updated successfully.\n");
}

void Update_Delete_FamilyDetails(TreenodeUser* root, TreenodeExpense* expenseroot, TreenodeFamily* familyRoot, int user_id){
    UserNode* userNode = GetUserById(root, user_id);
    if (userNode == NULL) {
        printf("Error: User with ID %d not found in node.\n", user_id);
        return ;
    }
    int choice;
    printf("\nChoose an option:\n");
    printf("1. Update Individual Details\n");
    printf("2. Delete Individual\n");
    printf("3. Delete Family\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > 3) {
        printf("Invalid choice. Please enter a number between 1 and 3.\n");
        return;
    }
    if (choice == 1) {
        UpdateUser(root, user_id);
    } else if (choice == 2) {
        deleteUser(root,user_id, expenseroot, familyRoot);
        printf("User with ID %d deleted successfully.\n", user_id);
    } else if (choice == 3) {
        deleteFamily(familyRoot, user_id, expenseroot);
        printf("Family with ID %d deleted successfully.\n", user_id);
    }
}

void Update_Delete_Expense(TreenodeUser* root, TreenodeExpense* expenseRoot, int user_id) {
    UserNode* userNode = GetUserById(root, user_id);
    if (userNode == NULL) {
        printf("Error: User with ID %d not found in node.\n", user_id);
        return ;
    }
    printf("Your expense ids are:\n");
    for (int i = 0; i < userNode->expense_count; i++) {
        printf("%d ", userNode->exp[i]);
    }
    printf("\n");
    printf("Enter expense id to update: ");
    int expense_id;
    scanf("%d", &expense_id);

    for (int i = 0; i < userNode->expense_count; i++) {
        if(userNode->exp[i] == expense_id) {
            break;
        }
    }

    int expensePos;
    TreenodeExpense* expenseNode = SearchTreeExpense(userNode->user_id,expense_id, expenseRoot, &expensePos);
    
    if (expenseNode != NULL && expensePos >= 1) {
        ExpenseNode* expense = &expenseNode->entry[expensePos];
        printf("Enter what you want to update\n");
        printf("1. Amount\n2. Category\n3. Date\n4. Delete Expense\n");
        int choice;
        scanf("%d", &choice);
        if (choice < 1 || choice > 4) {
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            return;
        }
        if (choice == 1) {
            printf("Enter new amount: ");
            float new_amount;
            scanf("%f", &new_amount);
            expense->amount = new_amount;
        } else if (choice == 2) {
            printf("Enter new category (0-Rent, 1-Grocery, 2-Utility, 3-Leisure, 4-Stationery, 5-Others): ");
            int new_category;
            scanf("%d", &new_category);
            if (new_category < 0 || new_category > 5) {
                printf("Error: Invalid category. Please enter a number between 0 and 5.\n");
                return;
            }
            expense->category = new_category;
        } else if (choice == 3) {
            printf("Enter new date (dd mm): ");
            int new_day, new_month;
            scanf("%d %d", &new_day, &new_month);
            if (new_day < 1 || new_day > 31 || new_month < 1 || new_month > 12) {
                printf("Error: Invalid date. Please enter a valid date.\n");
                return;
            }
            expense->date.date = new_day;
            expense->date.month = new_month;
        } else if (choice == 4) {
            DeleteExpense(user_id, expense_id, expenseRoot);
        }
        printf("Expense details updated successfully.\n");
    }
    return;
}
    
int main() {
    TreenodeUser* root = NULL;
    TreenodeFamily* familyRoot = NULL;
    TreenodeExpense* expenseRoot = NULL;
    LoadAllData(&root, &familyRoot, &expenseRoot);
    
    int choice;
    do {
        printf("\n--- Expense Tracking System ---\n");
        printf("1. Add User\n");
        printf("2. Add Expense\n");
        printf("3. Join Family\n");
        printf("4. Update/Delete Family details\n");
        printf("5. Update/Delete Expense\n");
        printf("6. Get Monthly Expense for your Family\n");
        printf("7. Get Categorical Expense\n");
        printf("8. Get Highest Expense Day\n");
        printf("9. Get Individual Expense\n");
        printf("10. Get Expense in Period\n");
        printf("11. Get Expense in Range\n");
        printf("12. Print Family and Print Users\n");
        printf("13. Exit\n");

        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch(choice) {
            case 1: {
                AddUser(&root, &familyRoot, expenseRoot);
                break;
            }
            case 2: {
                addExpense(root, &expenseRoot);
                break;
            }
            case 3: {
                int user_id, family_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                printf("Enter Family ID: ");
                scanf("%d", &family_id);
                JoinFamily(familyRoot, root, user_id, family_id, expenseRoot);
                break;
            }
            case 4: {
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                Update_Delete_FamilyDetails(root, expenseRoot, familyRoot, user_id);
                break;
            }
            case 5: {
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                Update_Delete_Expense(root, expenseRoot, user_id);
                break;
            }
            case 6: {
                int family_id;
                printf("Enter Family ID: ");
                scanf("%d", &family_id);
                GetMonthlyExpense(familyRoot, root, expenseRoot, family_id);
                break;
            }
            case 7: {
                int family_id, category;
                printf("Enter Family ID: ");
                scanf("%d", &family_id);
                printf("Enter Category (0-Rent, 1-Grocery, 2-Utility, 3-Leisure, 4-Stationery, 5-Others): ");
                scanf("%d", &category);
                if (category < 0 || category > 5) {
                    printf("Error: Invalid category. Please enter a number between 0 and 5.\n");
                    break;
                }
                GetCategoricalExpense(root, expenseRoot, familyRoot, family_id, category);
                break;
            }
            case 8: {
                int family_id;
                printf("Enter Family ID: ");
                scanf("%d", &family_id);
                GetHighestExpenseDay(familyRoot, root, expenseRoot, family_id);
                break;
            }
            case 9: {
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                GetIndividualExpense(root, expenseRoot, user_id);
                break;
            }
            case 10:{
                printf("Enter start date (dd mm): ");
                int start_day, start_month;
                scanf("%d %d", &start_day, &start_month);
                if(start_day < 1 || start_day > 31 || start_month < 1 || start_month > 12) {
                    printf("Error: Invalid date. Please enter a valid date.\n");
                }
                printf("Enter end date (dd mm): ");
                int end_day, end_month;
                scanf("%d %d", &end_day, &end_month); // assuming he has given correct date
                if(end_day < 1 || end_day > 31 || end_month < 1 || end_month > 12) {
                    printf("Error: Invalid date. Please enter a valid date.\n");
                }
                if(start_month > end_month || (start_month == end_month && start_day > end_day)) {
                    printf("Error: Start date should be earlier than end date.\n");
                }
                GetExpenseInPeriod(root, expenseRoot, start_day, start_month, end_day, end_month);
                break;
            }
            case 11:{
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                UserNode* userNode = GetUserById(root, user_id);
                if (userNode == NULL) {
                    break;
                }
                printf("User Name: %s\n", userNode->user_name);
                printf("Expense IDs: ");
                for (int i = 0; i < userNode->expense_count; i++) {
                    printf("%d ", userNode->exp[i]);
                }
                printf("\n");
                int exp1, exp2;
                printf("Enter expense range (exp1 exp2): ");
                scanf("%d %d", &exp1, &exp2);
                
                if(exp1 > exp2) {
                    printf("Error: Invalid range. Please enter a valid range.\n");
                }
                GetExpenseInRange(root, exp1, exp2, user_id, expenseRoot);
                break;
            }
            case 12: {
                // Print Users
                if (root == NULL) {
                    printf("No users found.\n");
                } else {
                    PrintTree(root);
                }
                break;
            }
            case 13: {
                // Exit and Save Data
                SaveAllData(root, familyRoot, expenseRoot);
                printf("Data saved. Exiting...\n");
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 13);

    return 0;
}

