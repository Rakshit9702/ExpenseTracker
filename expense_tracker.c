#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define NAME 100
#define MAX_USERS 1000
#define NUM_OF_FAMILIES 100
#define FAMILY_MEMBERS 4

typedef enum {
    true = 1,
    false = 0
} bool;

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

typedef struct FamilyNode_tag {
    int family_id;
    char family_name[NAME];
    float budget;
    float total_income;
    float total_expense;
    struct IndividualNode_tag* individuals[FAMILY_MEMBERS];
    int member_count;
    struct FamilyNode_tag *next;
} family_node;

typedef struct IndividualNode_tag {
    int user_id;
    char user_name[NAME];
    float income;
    struct IndividualNode_tag *next;
    family_node *family;
} individual_node;

typedef struct ExpenseNode_tag {
    int expense_id;
    individual_node* user;
    float amount;
    ExpenseCategory category;
    day_and_date date;
    struct ExpenseNode_tag *next;
} expense_node;

bool checkUserExists(individual_node *head, int user_id) {
    individual_node *temp = head->next;
    while(temp != NULL) {
        if(temp->user_id == user_id) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

bool NullPointer(void *ptr) {
    if (ptr == NULL) {
        printf("Memory allocation failed\n");
        return false;
    }
    return true;
}

// Function Prototypes
bool CreateFamily(family_node* dummyFamily, individual_node *head);
bool JoinFamily(family_node* dummyFamily, int family_id, individual_node *head, expense_node *dummyExpense);
bool AddUser(individual_node* dummyIndividual, individual_node *head, family_node *dummyFamily, expense_node *dummyExpense);
bool AddExpense(expense_node* dummyExpense, expense_node *head, family_node* dummyFamily);
void printFamily(family_node *head);
void PrintUsers(individual_node *head);
void updateDeleteExpense(expense_node* dummyExpense,  int expense_id,  individual_node* head);
void GetMonthlyExpense(family_node* dummyFamily, expense_node* dummyExpense, int family_id, int month);
void GetCategoricalExpense(expense_node* dummyExpense, int family_id);
void GetIndividualExpense(expense_node* dummyExpense, int user_id);
void initialize_individual(individual_node *individual, int user_id, char *user_name, float income, family_node *family);
void initialize_family(family_node *family, int family_id, char *family_name);
void initialize_expense(expense_node *expense, int expense_id, individual_node* user, float amount, ExpenseCategory category, day_and_date date);
void Get_highest_expense_day(expense_node* dummyExpense,family_node* dummyFamily,int family_id);
void UpdateOrDeleteIndividualFamilyDetails(individual_node* dummyIndividual, int user_id);

individual_node* findUserById(individual_node* dummyIndividual, int user_id) {
    individual_node *temp = dummyIndividual->next;
    while(temp != NULL) {
        if(temp->user_id == user_id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void loadDataFromFile(individual_node **dummyIndividual, family_node **dummyFamily, expense_node **dummyExpense) {
    FILE *file = fopen("data.txt", "r");
    if (!file) {
        printf("No previous data found. Starting fresh...\n");
        return;
    }

    char section[50];
    int user_id, family_id, expense_id, category, date, month;
    char user_name[NAME], family_name[NAME];
    float income, amount;

    while (fscanf(file, "%s", section) != EOF) {
        if (strcmp(section, "Users") == 0) {
            while (fscanf(file, "%d %s %f", &user_id, user_name, &income) == 3) {
                individual_node *newUser = malloc(sizeof(individual_node));
                initialize_individual(newUser, user_id, user_name, income, NULL);
                AddUser(*dummyIndividual, newUser, *dummyFamily, *dummyExpense);
            }
        }
        else if (strcmp(section, "Families") == 0) {
            while (fscanf(file, "%d %s %f %f", &family_id, family_name, &income, &amount) == 4) {
                family_node *newFamily = malloc(sizeof(family_node));
                initialize_family(newFamily, family_id, family_name);
                newFamily->total_income = income;
                newFamily->total_expense = amount;
                CreateFamily(newFamily, *dummyIndividual);
            }
        }
        else if (strcmp(section, "Memberships") == 0) {
            while (fscanf(file, "%d %d", &user_id, &family_id) == 2) {
                individual_node *user = findUserById(*dummyIndividual, user_id);
                if (user) JoinFamily(*dummyFamily, family_id, user, *dummyExpense);
            }
        }
        else if (strcmp(section, "Expenses") == 0) {
            while (fscanf(file, "%d %d %f %d %d %d", &expense_id, &user_id, &amount, &category, &date, &month) == 6) {
                individual_node *user = findUserById(*dummyIndividual, user_id);
                if (user) {
                    day_and_date exp_date = {date, month};
                    expense_node *newExpense = malloc(sizeof(expense_node));
                    initialize_expense(newExpense, expense_id, user, amount, (ExpenseCategory)category, exp_date);
                    AddExpense(*dummyExpense, newExpense, *dummyFamily);
                }
            }
        }
    }
    fclose(file);
    printf("Data loaded successfully!\n");
}

void saveDataToFile(individual_node *dummyIndividual, family_node *dummyFamily, expense_node *dummyExpense) {
    FILE *file = fopen("data.txt", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    fprintf(file, "Users\n");
    individual_node *user = dummyIndividual->next;
    while (user) {
        fprintf(file, "%d %s %.2f\n", user->user_id, user->user_name, user->income);
        user = user->next;
    }

    fprintf(file, "\nFamilies\n");
    family_node *family = dummyFamily->next;
    while (family) {
        fprintf(file, "%d %s %.2f %.2f\n", family->family_id, family->family_name, family->total_income, family->total_expense);
        family = family->next;
    }

    fprintf(file, "\nMemberships\n");
    user = dummyIndividual->next;
    while (user) {
        if (user->family) {
            fprintf(file, "%d %d\n", user->user_id, user->family->family_id);
        }
        user = user->next;
    }

    fprintf(file, "\nExpenses\n");
    expense_node *expense = dummyExpense->next;
    while (expense) {
        fprintf(file, "%d %d %.2f %d %d %d\n", expense->expense_id, expense->user->user_id, expense->amount, expense->category, expense->date.date, expense->date.month);
        expense = expense->next;
    }

    fclose(file);
    printf("Data saved successfully!\n");
}


void initialize_individual(individual_node *individual, int user_id, char *user_name, float income, family_node *family) {
    individual->user_id = user_id;
    strcpy(individual->user_name, user_name);
    individual->income = income;
    individual->family = family;
    individual->next = NULL;
}

void initialize_family(family_node *family, int family_id, char *family_name) {
    family->family_id = family_id;
    strcpy(family->family_name, family_name);
    family->total_income = 0;
    family->total_expense = 0;
    family->member_count = 0;
    family->budget = 0;
    for(int i = 0; i < FAMILY_MEMBERS; i++) {
        family->individuals[i] = NULL;
    }
    family->next = NULL;
}

void initialize_expense(expense_node *expense, int expense_id, individual_node* user, float amount, ExpenseCategory category, day_and_date date) { 
    expense->expense_id = expense_id;
    expense->user = user;
    expense->amount = amount;
    expense->category = category;
    expense->date = date;
    expense->next = NULL;
}

void PrintUsers(individual_node *head) {
    individual_node *temp = head->next;
    printf("Users:\n");
    while (temp != NULL) {
        printf("ID: %d, Name: %s, Income: %.2f\n", temp->user_id, temp->user_name, temp->income);
        temp = temp->next;
    }
}

void printFamily(family_node *head) {
    family_node *temp = head->next;
    
    while(temp != NULL) {
        printf("Family ID: %d, Name: %s, Total Income: %.2f, Total Expense: %.2f\n", 
               temp->family_id, temp->family_name, temp->total_income, temp->total_expense);
        
        printf("Family Members:\n");
        for(int i = 0; i < temp->member_count; i++) {
            if(temp->individuals[i] != NULL) {
                printf("  - User ID: %d, Name: %s, Income: %.2f\n", 
                       temp->individuals[i]->user_id, 
                       temp->individuals[i]->user_name, 
                       temp->individuals[i]->income);
            }
        }
        
        temp = temp->next;
    }
}

bool AddUser(individual_node* dummyIndividual, individual_node *head, family_node *dummyFamily, expense_node *dummyExpense) {
    individual_node* temp = dummyIndividual;
    
    if(!NullPointer(head)) return false;
    
    while(temp->next != NULL && temp->next->user_id < head->user_id) {
        temp = temp->next;
    }
    
    if(temp->next != NULL && temp->next->user_id == head->user_id) {
        printf("User already exists\n");
        return false;
    }
    
    head->next = temp->next;  
    temp->next = head;        

    int choice;
    printf("Enter 1 for Creating a family or 0 for Joining existing family: ");
    scanf("%d", &choice);
    
    if(choice == 1) {
        CreateFamily(dummyFamily, head);
    } else if(choice == 0) {
        PrintUsers(dummyIndividual);
        int family_id;
        printf("Enter Family ID: ");
        scanf("%d", &family_id);
        JoinFamily(dummyFamily, family_id, head, dummyExpense);
    } else {
        printf("Invalid choice\n");
    }
    
    return true;
}

bool CreateFamily(family_node* dummyFamily, individual_node *head) {
    family_node *temp = dummyFamily;
    family_node *family = malloc(sizeof(family_node));
    
    int family_id;
    printf("Enter Family ID: ");
    scanf("%d", &family_id);
    
    char family_name[NAME];
    printf("Enter Family Name: ");
    scanf("%s", family_name);
    
    initialize_family(family, family_id, family_name);
    
    family->individuals[0] = head;
    family->member_count = 1;
    family->total_income = head->income;
    head->family = family;
    
    while(temp->next != NULL && temp->next->family_id < family_id) {
        temp = temp->next;
    }
    
    family->next = temp->next;
    temp->next = family;
    
    return true;
}

bool JoinFamily(family_node* dummyFamily, int family_id, individual_node *head, expense_node *dummyExpense) {
    family_node *temp = dummyFamily->next;
    
    while(temp != NULL && temp->family_id != family_id) {
        temp = temp->next;
    }
    
    if(temp == NULL) {
        printf("Family does not exist\n");
        return false;
    }
    
    if(temp->member_count >= FAMILY_MEMBERS) {
        printf("Family is full, cannot add more members\n");
        return false;
    }
    
    temp->individuals[temp->member_count] = head;
    head->family = temp;
    temp->member_count++;
    temp->total_income += head->income;
    
    expense_node *expense_temp = dummyExpense->next;
    while (expense_temp != NULL) {
        if (expense_temp->user->user_id == head->user_id) {
            temp->total_expense += expense_temp->amount;
        }
        expense_temp = expense_temp->next;
    }
    return true;
}

bool AddExpense(expense_node* dummyExpense, expense_node *head, family_node* dummyFamily) {
    expense_node* temp = dummyExpense;
    
    if(!NullPointer(head)) return false;
    
    while (temp->next != NULL && 
           (temp->next->user->user_id < head->user->user_id || 
            (temp->next->user->user_id == head->user->user_id && temp->next->expense_id < head->expense_id))) {
        temp = temp->next;
    }
    
    if(temp->next != NULL && temp->next->user->user_id == head->user->user_id && temp->next->expense_id == head->expense_id) {
        printf("Expense already exists\n");
        return false;
    }
    
    // Find the family for this user and update total expense
    family_node *family_temp = dummyFamily->next;
    while(family_temp != NULL) {
        for(int i = 0; i < family_temp->member_count; i++) {
            if(family_temp->individuals[i] && family_temp->individuals[i]->user_id == head->user->user_id) {
                family_temp->total_expense += head->amount;
                break;
            }
        }
        family_temp = family_temp->next;
    }
    
    head->next = temp->next;
    temp->next = head;
    
    return true;
}

void updateDeleteExpense(expense_node* dummyExpense, int expense_id, individual_node* head) {
    // Input validation
    if (!dummyExpense || !head) {
        printf("Invalid input parameters\n");
        return;
    }

    expense_node *temp = dummyExpense->next;
    expense_node *prev = dummyExpense;
    
    // First, find the expense for the current user
    while (temp != NULL) {
        if (temp->user->user_id == head->user_id) {
            break;
        }
        prev = temp;
        temp = temp->next;
    }
    
    if (temp == NULL) {
        printf("No expenses found for this user\n");
        return;
    }
    
    // Then find the specific expense with the given ID
    while (temp != NULL && temp->expense_id != expense_id) {
        prev = temp;
        temp = temp->next;
        
        // Check if we're still looking at the same user's expenses
        if (temp && temp->user->user_id != head->user_id) {
            temp = NULL;
            break;
        }
    }
    
    if (temp == NULL) {
        printf("Expense with ID %d not found for this user\n", expense_id);
        return;
    }
    
    int choice;
    printf("Enter 1 for Update and 0 for Delete\n");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n'); // Clear input buffer
        return;
    }
    
    switch (choice) {
        case 1: { // Update
            float old_amount = temp->amount;
            float new_amount;
            printf("Enter new amount: ");
            if (scanf("%f", &new_amount) != 1) {
                printf("Invalid amount input\n");
                while (getchar() != '\n'); // Clear input buffer
                return;
            }
            
            temp->amount = new_amount;
            
            // Update family total if user belongs to a family
            if (temp->user && temp->user->family) {
                temp->user->family->total_expense += (new_amount - old_amount);
            }
            printf("Expense updated successfully\n");
            break;
        }
        
        case 0: { // Delete
            // Update family total if user belongs to a family
            if (temp->user && temp->user->family) {
                temp->user->family->total_expense -= temp->amount;
            }
            prev->next = temp->next;
            free(temp);
            printf("Expense deleted successfully\n");
            break;
        }
        
        default:
            printf("Invalid choice. Please enter 0 or 1\n");
            break;
    }
}

void GetMonthlyExpense(family_node* dummyFamily,expense_node* dummyExpense, int family_id, int month) {
    family_node *family = dummyFamily->next;
    
    // Find the family
    while(family != NULL && family->family_id != family_id) {
        family = family->next;
    }
    
    if(family == NULL) {
        printf("Family does not exist\n");
        return;
    }
    
    float monthly_expense = 0;
    
    // Traverse through each family member's expenses
    for(int i = 0; i < family->member_count; i++) {
        individual_node *individual = family->individuals[i];
        
        // Assuming you have a way to track expenses for each individual
        expense_node *expense = dummyExpense->next;
        while(expense != NULL) {
            if(expense->user->user_id == individual->user_id && expense->date.month == month) {
                monthly_expense += expense->amount;
            }
            expense = expense->next;
        }
    }
    
    printf("Total Monthly Expense for Family %d in Month %d: %.2f\n", family_id, month, monthly_expense);
    
    if(monthly_expense >= family->total_income) {
        printf("Expense is crossing the monthly income\n");
        printf("Difference is: %.2f\n", monthly_expense - family->total_income);
    } else {
        printf("Expense is within family's monthly income\n");
    }
    if (family->budget > 0) {
        printf("Family Budget for this month: %.2f\n", family->budget);
        if (monthly_expense > family->budget) {
            printf("Warning: Monthly expense exceeds the budget by %.2f\n", monthly_expense - family->budget);
        } else {
            printf("Monthly expense is within the budget.\n");
        }
    } else {
        printf("No budget set for this month.\n");
    }
}

void GetCategoricalExpense(expense_node* dummyExpense, int family_id) {
    family_node *family = NULL;
    individual_node *individuals[FAMILY_MEMBERS];
    int member_count = 0;
    float category_total[6] = {0};  //  Rent, Grocery, Utility, Leisure, Stationary, Other
    float individual_contributions[FAMILY_MEMBERS][6] = {0};

    // Find the family by ID
    expense_node *temp_expense = dummyExpense->next;
    while (temp_expense != NULL) {
        if (temp_expense->user->family->family_id == family_id) {
            family = temp_expense->user->family;
            break;
        }
        temp_expense = temp_expense->next;
    }

    if (family == NULL) {
        printf("Family with ID %d not found.\n", family_id);
        return;
    }

    // Store family members
    for (int i = 0; i < family->member_count; i++) {
        individuals[i] = family->individuals[i];
    }
    member_count = family->member_count;

    // Calculate individual contributions and total expense for each category
    temp_expense = dummyExpense->next;
    while (temp_expense != NULL) {
        for (int i = 0; i < member_count; i++) {
            if (temp_expense->user->user_id == individuals[i]->user_id) {
                int category = temp_expense->category;
                if (category >= 0 && category < 6) { 
                    individual_contributions[i][category] += temp_expense->amount;
                    category_total[category] += temp_expense->amount;
                }
            }
        }
        temp_expense = temp_expense->next;
    }

    // Print the results category-wise
    const char* category_names[] = {"Rent", "Grocery", "Utility", "Leisure", "Stationery","Other"};

    for (int cat = 0; cat < 6; cat++) {
        if(category_total[cat] == 0) {
            continue;
        }
        printf("\nTotal Expense for %s in Family ID %d: %.2f\n", category_names[cat], family_id, category_total[cat]);
        printf("Individual Contributions:\n");

        for (int i = 0; i < member_count; i++) {
            printf("User ID: %d, Name: %s, Contribution: %.2f\n", 
                   individuals[i]->user_id, 
                   individuals[i]->user_name, 
                   individual_contributions[i][cat]);
        }
    }

    // Print total expense across all categories
    float total_expense = 0;
    for (int cat = 0; cat < 6; cat++) {
        total_expense += category_total[cat];
    }
    printf("\nTotal Expense for Family ID %d: %.2f\n", family_id, total_expense);
}

void GetIndividualExpense(expense_node* dummyExpense, int user_id) {
    float category_expenses[6] = {0}; // One for each ExpenseCategory
    float total_expense = 0;
    
    expense_node *temp = dummyExpense->next;
    while(temp != NULL) {
        if(temp->user->user_id == user_id) {
            category_expenses[temp->category] += temp->amount;
            total_expense += temp->amount;
        }
        temp = temp->next;
    }
    
    // Sort categories by expense in descending order
    int sorted_indices[6];
    for(int i = 0; i < 6; i++) sorted_indices[i] = i;
    
    // Bubble sort for simplicity
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5 - i; j++) {
            if(category_expenses[sorted_indices[j]] < category_expenses[sorted_indices[j+1]]) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j+1];
                sorted_indices[j+1] = temp;
            }
        }
    }
    
    // Print total and category-wise expenses
    printf("Total Expense for User ID %d: %.2f\n", user_id, total_expense);
    printf("Expenses by Category (Descending Order):\n");
    
    for(int i = 0; i < 6; i++) {
        int category = sorted_indices[i];
        if(category_expenses[category] > 0) {
            switch(category) {
                case Rent: printf("Rent: "); break;
                case Grocery: printf("Grocery: "); break;
                case Utility: printf("Utility: "); break;
                case Leisure: printf("Leisure: "); break;
                case Stationary: printf("Stationery: "); break;
                case Others: printf("Others: "); break;
            }
            printf("%.2f\n", category_expenses[category]);
        }
    }
}

void Get_highest_expense_day(expense_node* dummyExpense,family_node* dummyFamily,int family_id){
    expense_node* expense = dummyExpense->next;
    family_node* family = dummyFamily->next;

    int found = 0;
    while(!found && family != NULL){
        if(family->family_id == family_id){
            found = 1;
        }
        else{
            family = family->next;
        }
    }
    if(!found){
        printf("Family doesn't exist!!\n");
    }
    else{
            typedef struct {
            int date;
            int month;
            float total_expense;
        } daily_expense;

        // Assuming max 10 days & 12 months
        daily_expense dailyExpenses[120] = {0};

        // Summing expenses for each date
        while (expense != NULL) {
            if (expense->user->family->family_id == family_id) {
                if (expense->date.date >= 1 && expense->date.date <= 10 && expense->date.month >= 1 && expense->date.month <= 12) {
                    int index = (expense->date.month - 1) * 10 + (expense->date.date - 1);
                    dailyExpenses[index].date = expense->date.date;
                    dailyExpenses[index].month = expense->date.month;
                    dailyExpenses[index].total_expense += expense->amount;
                }
            }
            expense = expense->next;
        }

        // Finding the day with the highest total expense
        float max_expense = 0;
        int max_date = 0;
        int max_month = 0;

        for (int i = 0; i < 10 * 12; i++) {
            if (dailyExpenses[i].total_expense > max_expense) {
                max_expense = dailyExpenses[i].total_expense;
                max_date = dailyExpenses[i].date;
                max_month = dailyExpenses[i].month;
            }
        }

        if (max_expense > 0) {
            printf("Highest expense for Family ID %d: %.2f on %d/%d\n", family_id, max_expense, max_date, max_month);
        } else {
            printf("No expenses found for Family ID %d.\n", family_id);
        }
    }
}

void UpdateOrDeleteIndividualFamilyDetails(individual_node* dummyIndividual, int user_id){
    individual_node* temp = dummyIndividual->next;
    individual_node* prev = dummyIndividual;

    while(temp != NULL && temp->user_id != user_id){
        prev = temp;
        temp = temp->next;
    }
    if(temp == NULL){
        printf("User not found!!\n");
        return ;
    }
    else{
        int choice;
        printf("Enter 1 for Update and 0 for Delete: ");
        scanf("%d", &choice);
        if(choice == 1){
            float new_income;
            printf("Enter new income: ");
            scanf("%f", &new_income);
            temp->family->total_income -= temp->income;
            temp->income = new_income;
            temp->family->total_income += temp->income;
            
        }
        else if(choice == 0){
            temp->family->member_count--;
            temp->family->total_income -= temp->income;
            for(int i = 0;i<FAMILY_MEMBERS;i++){
                if(temp->family->individuals[i]->user_id == temp->user_id) 
                free(temp->family->individuals[i]);
            }
            prev->next = temp->next;
            free(temp);
        }
        else{
            printf("Invalid choice!\n");
        }
    }
}

int main() {
    day_and_date dummyDate = {-1, -1};

    individual_node *dummyIndividual = malloc(sizeof(individual_node));
    initialize_individual(dummyIndividual, -1, "dummy", -1, NULL);

    family_node *dummyFamily = malloc(sizeof(family_node));
    initialize_family(dummyFamily, -1, "dummy");

    expense_node *dummyExpense = malloc(sizeof(expense_node));
    initialize_expense(dummyExpense, -1, NULL, -1, Others, dummyDate);

    loadDataFromFile(&dummyIndividual, &dummyFamily, &dummyExpense);

    int choice;
    do {
        printf("\n--- Expense Tracking System ---\n");
        printf("1. Add User\n");
        printf("2. Add Expense\n");
        printf("3. Join Family\n");
        printf("4. Update/Delete Family details\n");
        printf("5. Update/Delete Expense\n");
        printf("6. Get Total Expense\n");
        printf("7. Get Categorical Expense\n");
        printf("8. Get Highest Expense Day\n");
        printf("9. Get Individual Expense\n");
        printf("10. Set Budget");
        printf("11. Print Family and Print users\n"); 
        printf("12. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: {
                int user_id;
                char user_name[NAME];
                float income;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                printf("Enter Name: ");
                scanf("%s", user_name);
                printf("Enter Income: ");
                scanf("%f", &income);
                
                individual_node *head = malloc(sizeof(individual_node));
                initialize_individual(head, user_id, user_name, income, NULL);
                
                if(AddUser(dummyIndividual, head, dummyFamily, dummyExpense)) {
                    printf("User added successfully\n");
                } else {
                    free(head);
                    printf("User addition failed\n");
                }
                break;
            }
            case 2: {
                int expense_id, user_id;
                float amount;
                ExpenseCategory category;
                day_and_date date;
                
                printf("Enter Expense ID: ");
                scanf("%d", &expense_id);
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                printf("Enter Amount: ");
                scanf("%f", &amount);
                printf("Enter Category (0-Rent, 1-Grocery, 2-Utility, 3-Leisure, 4-Stationery, 5-Others): ");
                scanf("%d", &category);
                printf("Enter date (date month): ");
                scanf("%d %d", &date.date, &date.month);
                individual_node *user = findUserById(dummyIndividual, user_id);
                
                expense_node *head = malloc(sizeof(expense_node));
                initialize_expense(head, expense_id, user, amount, category, date);
                
                if(AddExpense(dummyExpense, head, dummyFamily)) {
                    printf("Expense added successfully\n");
                } else {
                    free(head);
                    printf("Expense addition failed\n");
                }
                break;
            }
            case 3: {
                int user_id;
                int family_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                printf("Enter Family ID: ");
                scanf("%d", &family_id);
                individual_node *head = findUserById(dummyIndividual, user_id);
                if(JoinFamily(dummyFamily, family_id, head, dummyExpense)) {
                    printf("User joined family successfully\n");
                } else {
                    printf("User joining family failed\n");
                }
                break;
            }
            case 4:{
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                UpdateOrDeleteIndividualFamilyDetails(dummyIndividual, user_id);
                break;
            }
            case 5:{
                int expense_id;
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                printf("Enter Expense ID: ");
                scanf("%d", &expense_id);
                individual_node *head = findUserById(dummyIndividual, user_id);
                updateDeleteExpense(dummyExpense, expense_id, head);
                break;
            }
            case 6:{
                int family_id, month;
                printf("Enter Family ID: ");
                scanf("%d", &family_id);
                printf("Enter Month: ");
                scanf("%d", &month);
                GetMonthlyExpense(dummyFamily,dummyExpense, family_id, month);
                break;
            }
            case 7:{
                int family_id;
                printf("Enter Family ID: ");
                scanf("%d", &family_id);
                GetCategoricalExpense(dummyExpense, family_id);
                break;
            }
            case 8:{
                int family_id;
                printf("Enter Family ID: ");
                scanf("%d",&family_id);
                Get_highest_expense_day(dummyExpense,dummyFamily,family_id);
                break;
            }
            case 9:{
                int user_id;
                printf("Enter User ID: ");
                scanf("%d", &user_id);
                GetIndividualExpense(dummyExpense, user_id);
                break;
            }
            case 10:{
                float budget = 0;
                int family_id;
                printf("Enter family id:");
                scanf("%d", &family_id);
                family_node *family = dummyFamily->next;
                while(family != NULL && family->family_id != family_id) {
                    family = family->next;
                }
                if(family == NULL) {
                    printf("Family not found!\n");
                } else {
                    printf("Enter budget for family %s: ", family->family_name);
                    scanf("%f", &budget);
                    family->budget = budget;
                    printf("Budget set successfully for family %s\n", family->family_name);
                }
            }
            case 11:{
                PrintUsers(dummyIndividual);
                printf("\n");
                printFamily(dummyFamily);
                break;
            }
            case 12: {
                printf("Exiting...\n");
                saveDataToFile(dummyIndividual, dummyFamily, dummyExpense);
                break;
            }
            default: {
                printf("Invalid choice\n");
                break;
            }
        }
    } while (choice != 11);

    // Free allocated memory (simplified)
    free(dummyIndividual);
    free(dummyFamily);
    free(dummyExpense);

    return 0;
}
