#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <float.h>
#include <limits.h>
#ifdef _WIN32
#include <conio.h>
#elif defined(__APPLE__) && defined(__MACH__)
#include <unistd.h>
#include <objc/objc.h>
#endif
#define RED "\033[1;31m"
#define RESET "\033[0m"
#define INITIAL_CAPACITY 10
#define MAX_USERNAME_LENGTH 100
#define MAX_PASSWORD_LENGTH 100
#define MAX_ANSWER_LENGTH 100
#define MAX_PROJECT_TYPE_LENGTH 100
#define MAX_DATE_LENGTH 20
#define MAX_TRANSACTIONS 100
#define MAX_LINE_LENGTH 256
#define MAX_EMAIL_LENGTH 100

typedef struct
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    char answer_security_question1[MAX_ANSWER_LENGTH];
    char answer_security_question2[MAX_ANSWER_LENGTH];
    char answer_security_question3[MAX_ANSWER_LENGTH];
    float carbon_credit;
    float balance;
    char order_status[50];
    char buyer_name[MAX_USERNAME_LENGTH]; // เปลี่ยนกับ trans
    int order_amount;
    char email[MAX_EMAIL_LENGTH];
} User;

typedef struct
{
    int project_id;
    char project_name[256];
    char project_description[512];
    char project_type[MAX_PROJECT_TYPE_LENGTH];
    float carbon_credit;
    float price;
    int project_duration;
    char environmental_impact[512];
    char related_documents[256];
    char contact_info[256];
    char username[MAX_USERNAME_LENGTH];
    char status[50];
} Project;

typedef struct
{
    int tProject_id;
    char tProject_name[256];
    char tSeller_name[MAX_USERNAME_LENGTH];
    char tBuyer_name[MAX_USERNAME_LENGTH];
    char tStatus[50];
} Transaction;

Transaction tTransactions[MAX_TRANSACTIONS]; // อาร์เรย์เก็บข้อมูลการทำธุรกรรม
int tTransaction_count = 0;                  // ตัวแปรเก็บจำนวนธุรกรรมที่โหลด
User *users = NULL;
Project *projects = NULL;
int user_count = 0, user_capacity = INITIAL_CAPACITY;
int project_count = 0, project_capacity = INITIAL_CAPACITY;
int project_id_counter = 1;
int current_user_index = -1;
User *filtered_users = NULL;
int filtered_count = 0;
void clear_screen();
void save_user_to_file();
void load_users_from_file();
void load_projects_from_file();
int user_login();
int user_register();
int user_forget_password(char *username);
int buyer_menu();
void seller_menu();
void buy_from_filtered(char project_id_input[10]);
void confirm_project_details(char *project_name, char *project_description, char *project_type, float *carbon_credit, float *price, int *project_duration, char *environmental_impact, char *related_documents, char *contact_info);
void edit_project();
void edit_profile();
int search_projects();
int reset_password(char *username, int user_count);
int filter_projects(const char *project_type, float min_carbon_credit, float max_carbon_credit, float min_price, float max_price, int min_duration, int max_duration);
void safe_input(char *buffer, int max_length);
void get_password(char *password, int max_length);
void check_order_status();
void main_menu();
void save_all_projects_to_file();
void add_new_project();
void delete_project();
void save_new_project(Project *new_project);
void transaction(int tProject_id, char tProject_name[], char tSeller_name[], char tBuyer_name[], char tStatus[]);
void load_transactions();
int validate_project_selection(int project_id, const char *current_user_name);
int Bvalidate_project_selection(int project_id, const char *current_user_name);
int delete_project_selection(int project_id, const char *current_user_name);
void delete_transaction(int project_id);
void save_transaction();
bool can_edit_project(int project_id);
int find_name_in_transaction(const char *username);

int find_name_in_transaction(const char *username)
{
    FILE *file = fopen("transaction.txt", "r"); // เปิดไฟล์ในโหมดอ่าน
    if (file == NULL)
    {
        printf("Unable to open transaction file\n");
        return 0; // ถ้าไม่สามารถเปิดไฟล์ได้, คืนค่า 0
    }

    char line[MAX_LINE_LENGTH]; // buffer สำหรับเก็บข้อมูลแต่ละบรรทัด
    int found = 0;

    // อ่านไฟล์ทีละบรรทัด
    while (fgets(line, sizeof(line), file))
    {
        int project_id;
        char project_name[100], seller_name[50], buyer_name[50], status[20];

        // สมมุติว่าไฟล์ใช้รูปแบบ CSV: Project ID, Project Name, Seller Name, Buyer Name, Status
        if (sscanf(line, "%d,%99[^,],%49[^,],%49[^,],%19[^\n]", &project_id, project_name, seller_name, buyer_name, status) == 5)
        {
            // เปรียบเทียบ buyer_name หรือ seller_name กับ username ที่เราค้นหา
            if (strcmp(buyer_name, username) == 0 || strcmp(seller_name, username) == 0)
            {
                found = 1; // พบชื่อในไฟล์
                break;     // ออกจากลูปหากพบ
            }
        }
    }

    fclose(file); // ปิดไฟล์หลังจากอ่านเสร็จ

    return found;
}

void save_transaction()
{
    // Open the file in read mode to check existing transactions
    FILE *file = fopen("transaction.txt", "r");
    if (file == NULL)
    {
        printf("Unable to open file for reading. Creating a new file.\n");
        file = fopen("transaction.txt", "w");
        if (file == NULL)
        {
            printf("Unable to create a new transaction file.\n");
            return;
        }
        fclose(file);
        file = fopen("transaction.txt", "r");
    }

    // Read the existing transactions into a local array
    Transaction existing_transactions[MAX_TRANSACTIONS];
    int existing_count = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file))
    {
        Transaction t;
        // Check if the line matches the expected format
        if (sscanf(line, "%d,%255[^,],%99[^,],%99[^,],%49[^\n]",
                   &t.tProject_id, t.tProject_name, t.tSeller_name,
                   t.tBuyer_name, t.tStatus) == 5)
        {
            existing_transactions[existing_count++] = t;
        }
    }

    fclose(file);

    // Merge the changes from tTransactions[] with existing transactions
    for (int i = 0; i < tTransaction_count; i++)
    {
        bool found = false;
        for (int j = 0; j < existing_count; j++)
        {
            if (tTransactions[i].tProject_id == existing_transactions[j].tProject_id)
            {
                // Update the existing transaction if changes are found
                if (strcmp(tTransactions[i].tProject_name, existing_transactions[j].tProject_name) != 0 ||
                    strcmp(tTransactions[i].tSeller_name, existing_transactions[j].tSeller_name) != 0 ||
                    strcmp(tTransactions[i].tBuyer_name, existing_transactions[j].tBuyer_name) != 0 ||
                    strcmp(tTransactions[i].tStatus, existing_transactions[j].tStatus) != 0)
                {
                    existing_transactions[j] = tTransactions[i];
                }
                found = true;
                break;
            }
        }
        // If not found, add the new transaction
        if (!found)
        {
            existing_transactions[existing_count++] = tTransactions[i];
        }
    }

    // Open the file in write mode to overwrite it with updated data
    file = fopen("transaction.txt", "w");
    if (file == NULL)
    {
        printf("Unable to open file for saving transactions\n");
        return;
    }

    // Write the merged transactions back to the file
    for (int i = 0; i < existing_count; i++)
    {
        fprintf(file, "%04d,%s,%s,%s,%s\n",
                existing_transactions[i].tProject_id,
                existing_transactions[i].tProject_name,
                existing_transactions[i].tSeller_name,
                existing_transactions[i].tBuyer_name,
                existing_transactions[i].tStatus);
    }

    fclose(file);
}

void delete_transaction(int project_id)
{
    // Check if there are any transactions
    if (tTransaction_count == 0)
    {
        printf("No transactions to delete\n");
        return;
    }

    // Search and delete transaction matching the project_id
    for (int i = 0; i < tTransaction_count; i++)
    {
        if (tTransactions[i].tProject_id == project_id)
        {
            // Move remaining data up to fill the gap
            for (int j = i; j < tTransaction_count - 1; j++)
            {
                tTransactions[j] = tTransactions[j + 1];
            }

            // Reduce transaction count
            tTransaction_count--;

            // Save updated data to file
            FILE *file = fopen("transaction.txt", "w");
            if (file == NULL)
            {
                printf("Unable to open file for saving\n");
                return;
            }

            // Write new data to file
            for (int k = 0; k < tTransaction_count; k++)
            {
                fprintf(file, "%04d,%s,%s,%s,%s\n",
                        tTransactions[k].tProject_id,
                        tTransactions[k].tProject_name,
                        tTransactions[k].tSeller_name,
                        tTransactions[k].tBuyer_name,
                        tTransactions[k].tStatus);
            }

            fclose(file);

            printf("Transaction deleted successfully\n");
            return;
        }
    }

    // If project_id not found
    printf("No transaction found with this Project ID\n");
}

int manage_project_selection(int project_id, const char *current_user_name)
{
    bool valid_project = false;
    while (!valid_project)
    {
        printf("\nEnter the project ID to manage (or 0 to go back): ");
        scanf("%d", &project_id);
        getchar(); // Consume newline character

        if (project_id == 0)
        {
            return 0; // Exit if user enters 0 to go back
        }

        // Search through transactions to find the matching project_id
        for (int i = 0; i < project_count; i++)
        {
            if (strcmp(projects[i].username, users[current_user_index].username) == 0)
            {
                // Check if the seller name matches the current user's name
                if (strcmp(projects[i].status, "Pending") == 0)
                {
                    valid_project = true; // Valid selection found
                    break;
                }
                else
                {
                    printf("Invalid.\n");
                    break;
                }
            }
        }
        if (!valid_project)
        {
            printf("Error: Invalid Project ID or seller mismatch. Please try again.\n");
        }
    }

    return project_id; // Return the valid project_id
}

int delete_project_selection(int project_id, const char *current_user_name)
{
    bool valid_project = false;
    while (!valid_project)
    {
        printf("\nEnter the project ID to delete (or 0 to go back): ");
        scanf("%d", &project_id);
        getchar(); // Consume newline character

        if (project_id == 0)
        {
            return 0; // Exit if user enters 0 to go back
        }

        // Search through transactions to find the matching project_id
        for (int i = 0; i < project_count; i++)
        {
            if (projects[i].project_id == project_id)
            {
                // Check if the seller name matches the current user's name
                if (strcmp(projects[i].username, current_user_name) == 0)
                {
                    valid_project = true; // Valid selection found
                    break;
                }
                else
                {
                    printf("Error: The seller name does not match the current user's name.\n");
                    break;
                }
            }
        }
        if (!valid_project)
        {
            printf("Error: Invalid Project ID or seller mismatch. Please try again.\n");
        }
    }

    return project_id; // Return the valid project_id
}

int validate_project_selection(int project_id, const char *current_user_name)
{
    bool valid_project = false;
    while (!valid_project)
    {
        printf("\nEnter the Project ID to manage (or 0 to go back): ");
        scanf("%d", &project_id);
        getchar(); // Consume newline character

        if (project_id == 0)
        {
            return 0; // Exit if user enters 0 to go back
        }

        // Search through transactions to find the matching project_id
        for (int i = 0; i < tTransaction_count; i++)
        {
            if (tTransactions[i].tProject_id == project_id)
            {
                // Check if the seller name matches the current user's name
                if (strcmp(tTransactions[i].tSeller_name, current_user_name) == 0)
                {
                    valid_project = true; // Valid selection found
                    break;
                }
                else
                {
                    printf("Error: The seller name does not match the current user's name.\n");
                    break;
                }
            }
        }

        if (!valid_project)
        {
            printf("Error: Invalid Project ID or seller mismatch. Please try again.\n");
        }
    }

    return project_id; // Return the valid project_id
}

int Bvalidate_project_selection(int project_id, const char *current_user_name)
{
    bool valid_project = false;
    while (!valid_project)
    {
        printf("\nEnter the Project ID to acknowledge the order (or 0 to go back): ");
        scanf("%d", &project_id);
        getchar(); // Consume newline character

        if (project_id == 0)
        {
            return 0; // Exit if user enters 0 to go back
        }

        // Search through transactions to find the matching project_id
        for (int i = 0; i < tTransaction_count; i++)
        {
            if (tTransactions[i].tProject_id == project_id)
            {
                // Check if the seller name matches the current user's name
                if (strcmp(tTransactions[i].tBuyer_name, current_user_name) == 0)
                {
                    valid_project = true; // Valid selection found
                    break;
                }
                else
                {
                    printf("Error: The buyer name does not match the current user's name.\n");
                    break;
                }
            }
        }

        if (!valid_project)
        {
            printf("Error: Invalid Project ID or seller mismatch. Please try again.\n");
        }
    }

    return project_id; // Return the valid project_id
}

void transaction(int tProject_id, char tProject_name[], char tSeller_name[], char tBuyer_name[], char tStatus[])
{
    FILE *file = fopen("transaction.txt", "a"); // เปิดไฟล์ในโหมดเพิ่มข้อมูล
    if (file == NULL)
    {
        // ถ้าไฟล์ไม่มี ให้สร้างใหม่
        file = fopen("transaction.txt", "w");
        if (file == NULL)
        {
            printf("Error opening or creating the transaction file! \n");
            return;
        }
        printf("File does not exist. Creating a new transaction file.\n");
    }

    // เพิ่มข้อมูลลงในไฟล์
    fprintf(file, "%04d,%s,%s,%s,%s\n", tProject_id, tProject_name, tSeller_name, tBuyer_name, tStatus);

    fclose(file);
}

void load_transactions()
{
    FILE *file = fopen("transaction.txt", "r");
    if (file == NULL)
    {
        printf("Error opening transaction file! \n");
        return;
    }

    // อ่านข้อมูลจากไฟล์
    while (fscanf(file, "%d,%99[^,],%99[^,],%99[^,],%49[^\n]\n",
                  &tTransactions[tTransaction_count].tProject_id,
                  tTransactions[tTransaction_count].tProject_name,
                  tTransactions[tTransaction_count].tSeller_name,
                  tTransactions[tTransaction_count].tBuyer_name,
                  tTransactions[tTransaction_count].tStatus) == 5)
    {
        tTransaction_count++; // เพิ่มจำนวนธุรกรรมที่โหลด
        if (tTransaction_count >= MAX_TRANSACTIONS)
        {
            printf("Reached max transaction limit!\n");
            break;
        }
    }

    fclose(file);
}

void save_new_project(Project *new_project)
{
    FILE *file = fopen("projects.txt", "r");
    if (file == NULL)
    {

        printf("Error opening project file! Creating a new file...\n");

        file = fopen("projects.txt", "w");
        if (file == NULL)
        {
            printf("Error: Unable to create projects.txt.\n");
            return;
        }
        fclose(file);
        file = fopen("projects.txt", "r");
    }

    int last_project_id = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file))
    {
        int project_id;

        if (sscanf(buffer, "\"%d\",", &project_id) == 1)
        {
            if (project_id > last_project_id)
            {
                last_project_id = project_id;
            }
        }
    }

    new_project->project_id = last_project_id + 1;

    if (last_project_id == 0 && ftell(file) == 0)
    {
        new_project->project_id = 1;
    }

    fclose(file);

    file = fopen("projects.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    const char *default_value = "none";
    const char *project_name = (new_project->project_name[0] != '\0') ? new_project->project_name : default_value;
    const char *project_description = (new_project->project_description[0] != '\0') ? new_project->project_description : default_value;
    const char *project_type = (new_project->project_type[0] != '\0') ? new_project->project_type : default_value;
    const char *environmental_impact = (new_project->environmental_impact[0] != '\0') ? new_project->environmental_impact : default_value;
    const char *related_documents = (new_project->related_documents[0] != '\0') ? new_project->related_documents : default_value;
    const char *contact_info = (new_project->contact_info[0] != '\0') ? new_project->contact_info : default_value;

    const char *username = users[current_user_index].username;

    const char *status = "Available";

    fprintf(file, "\"%04d\",\"%s\",\"%s\",\"%s\",%.2f,%.2f,%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
            new_project->project_id,
            project_name, project_description,
            project_type, new_project->carbon_credit,
            new_project->price, new_project->project_duration,
            environmental_impact, related_documents, contact_info, username, status);

    fclose(file);
}

void save_all_projects_to_file()
{
    FILE *file = fopen("projects.txt", "w");
    if (!file)
    {
        printf("Error: Unable to open 'projects.txt' for writing.\n");
        return;
    }
    for (int i = 0; i < project_count; i++)
    {
        fprintf(file, "\"%04d\",\"%s\",\"%s\",\"%s\",%.2f,%.2f,%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
                projects[i].project_id, projects[i].project_name, projects[i].project_description,
                projects[i].project_type, projects[i].carbon_credit, projects[i].price,
                projects[i].project_duration, projects[i].environmental_impact,
                projects[i].related_documents, projects[i].contact_info, projects[i].username, projects[i].status);
    }
    fclose(file);
}
void main_menu()
{
    while (1)
    {
        clear_screen();
        printf("=== Main Menu ===\n");
        printf("1. Buyer Menu\n");
        printf("2. Seller Menu\n");
        bool has_pending_order = false;
        if (strcmp(users[current_user_index].order_status, "Cancelled") == 0 || strcmp(users[current_user_index].order_status, "Pending") == 0 || strcmp(users[current_user_index].order_status, "Complete") == 0)
        {
            has_pending_order = true;
        }
        for (int i = 0; i < project_count; i++)
        {
            if (find_name_in_transaction(users[current_user_index].username)!= 0 )
            {
                has_pending_order = true;
                break;
            }
        }
        if (has_pending_order)
        {
            printf(RED "3. Status (Check Order)\n" RESET);
        }
        else
        {
            printf("3. Status\n");
        }
        printf("4. Edit Profile\n");
        printf("5. Logout\n");
        printf("=================\n");
        printf("Choose an option: ");
        int choice;
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            buyer_menu();
            break;

        case 2:
            seller_menu();
            break;
        case 3:
            if (has_pending_order)
            {
                clear_screen();
                load_transactions();
                check_order_status();
            }
            else
            {
                printf("You currently have no pending orders.\n");
                getchar();
            }
            break;
        case 4:
            edit_profile();
            break;
        case 5:
            printf("Logging out...\n");
            return;
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            getchar();
            break;
        }
    }
}
void check_order_status()
{
    while (1)
    {

        clear_screen();
        printf("=== Check Order Status ===\n");
        bool has_seller_pending_orders = false;
        bool has_buyer_acknowledgment_orders = false;
        for (int i = 0; i < tTransaction_count; i++)
        {
            if (strcmp(tTransactions[i].tSeller_name, users[current_user_index].username) == 0 && strcmp(tTransactions[i].tStatus, "Pending") == 0)

            {
                has_seller_pending_orders = true;
                break;
            }
        }
        for (int i = 0; i < tTransaction_count; i++)
        {
            if (strcmp(users[current_user_index].order_status, "Pending") == 0 && strcmp(users[current_user_index].username, tTransactions[i].tBuyer_name) == 0)
            {
                has_buyer_acknowledgment_orders = true;
                break;
            }
        }
        printf("1. Orders waiting for seller confirmation ");
        if (has_seller_pending_orders)
        {
            printf("\033[1;31m[!]\033[0m\n");
        }
        else
        {
            printf("\n");
        }
        printf("2. Orders waiting for your acknowledgment ");

        if (has_buyer_acknowledgment_orders)
        {
            printf("\033[1;31m[!]\033[0m\n");
        }
        else
        {
            printf("\n");
        }
        printf("3. Go Back\n");
        printf("Choose an option: ");
        int choice;
        scanf("%d", &choice);
        getchar();
        if (choice == 1)
        {
            clear_screen();
            printf("=== Orders Waiting for Seller Confirmation ===\n");
            bool has_pending_orders = false;
            for (int i = 0; i < project_count; i++)
            {
                bool is_project_shown = false;
                for (int j = 0; j < tTransaction_count; j++)
                {
                    if (strcmp(tTransactions[j].tSeller_name, users[current_user_index].username) == 0 &&
                        strcmp(projects[i].status, "Pending") == 0 && projects[i].project_id == tTransactions[j].tProject_id)
                    {
                        if (!is_project_shown) // ตรวจสอบว่าโปรเจกต์นี้ยังไม่แสดงผล
                        {
                            is_project_shown = true;
                            has_pending_orders = true;
                            printf("\n===========================\n");
                            printf("Project ID: %04d\n", projects[i].project_id);
                            printf("Name: %s\n", projects[i].project_name);
                            printf("Price: %.2f\n", projects[i].price);
                            printf("Buyer: %s\n", tTransactions[j].tBuyer_name);
                            printf("Status: Pending Confirmation\n");
                            printf("===========================\n");
                        }
                    }
                }
            }
            if (!has_pending_orders)
            {
                printf("\nNo pending orders for confirmation.\n");
                getchar();
                continue;
            }

            int project_id;
            project_id = validate_project_selection(project_id, users[current_user_index].username);
            if (project_id == 0)
            {
                continue;
            }
            for (int i = 0; i < project_count; i++)
            {
                if (projects[i].project_id == project_id &&
                    strcmp(projects[i].username, users[current_user_index].username) == 0 &&
                    strcmp(projects[i].status, "Pending") == 0)
                {
                    printf("\nYou selected Project ID: %04d\n", project_id);
                    int seller_choice;
                    printf("\nOptions:\n");
                    printf("1. Confirm Order\n");
                    printf("2. Cancel Order\n");
                    printf("3. Back\n");
                    printf("Choose: ");
                    scanf("%d", &seller_choice);
                    getchar();
                    if (seller_choice == 1)
                    {
                        for (int j = 0; j < tTransaction_count; j++)
                        {
                            for (int k = 0; k < user_count; k++)
                            {
                                if (projects[i].project_id == tTransactions[j].tProject_id && strcmp(tTransactions[j].tBuyer_name, users[k].username) == 0)
                                {
                                    users[k].carbon_credit += projects[i].carbon_credit;
                                    strcpy(tTransactions[j].tStatus, "Complete");
                                    users[current_user_index].balance += projects[i].price;
                                    save_user_to_file();
                                }
                                if (find_name_in_transaction(users[current_user_index].username) <= 1)
                                {
                                    strcpy(users[current_user_index].order_status, "none");
                                    strcpy(users[current_user_index].buyer_name, "none"); // change
                                    users[current_user_index].order_amount = 0;
                                    save_user_to_file();
                                }
                            }
                        }
                        save_transaction();
                        strcpy(projects[i].status, "Complete");
                        save_all_projects_to_file();
                    }
                    else if (seller_choice == 2)
                    {
                        for (int j = 0; j < user_count; j++)
                        {
                            for (int k = 0; k < user_count; k++)
                            {
                                if (projects[i].project_id == tTransactions[j].tProject_id && strcmp(tTransactions[j].tBuyer_name, users[k].username) == 0)
                                {
                                    users[k].balance += projects[i].price;
                                    strcpy(tTransactions[j].tStatus, "Cancelled");
                                    save_transaction();
                                }
                                if (find_name_in_transaction(users[current_user_index].username) <= 1)
                                {
                                    strcpy(users[current_user_index].order_status, "none");
                                    strcpy(users[current_user_index].buyer_name, "none"); // change
                                    users[current_user_index].order_amount = 0;
                                    save_user_to_file();
                                }
                            }
                        }

                        strcpy(projects[i].status, "Available");
                        save_all_projects_to_file();
                    }
                    else
                    {
                        printf("\nReturning to previous menu.\n");
                        getchar();
                    }
                }
            }
        }
        else if (choice == 2)
        {
            clear_screen();
            printf("=== Orders Waiting for Your Acknowledgment ===\n");

            bool has_acknowledge_orders = false;
            for (int i = 0; i < project_count; i++)
            {
                bool is_project_shown = false;
                for (int j = 0; j < tTransaction_count; j++)
                {
                    if (strcmp(users[current_user_index].order_status, "none") != 0 &&
                        strcmp(tTransactions[j].tBuyer_name, users[current_user_index].username) == 0 &&
                        tTransactions[j].tProject_id == projects[i].project_id)
                    {
                        if (!is_project_shown)
                        {
                            is_project_shown = true;
                            has_acknowledge_orders = true;
                            printf("\n===========================\n");
                            printf("Project ID: %04d\n", projects[i].project_id);
                            printf("Name: %s\n", projects[i].project_name);
                            printf("Price: %.2f\n", projects[i].price);
                            printf("Buyer: %s\n", tTransactions[j].tBuyer_name);
                            printf("Status: %s\n", tTransactions[j].tStatus);
                            printf("===========================\n");
                        }
                    }
                }
            }

            if (!has_acknowledge_orders)
            {
                printf("\nNo orders waiting for your acknowledgment.\n");
                getchar();
                continue;
            }

            int project_id;
            project_id = Bvalidate_project_selection(project_id, users[current_user_index].username);

            if (project_id == 0)
            {
                continue;
            }

            for (int i = 0; i < project_count; i++)
            {
                for (int j = 0; j < tTransaction_count; j++)
                {
                    if (projects[i].project_id == project_id &&
                        tTransactions[j].tProject_id == projects[i].project_id &&
                        strcmp(tTransactions[j].tStatus, "Complete") == 0)
                    {

                        delete_transaction(project_id);
                        printf("\nYou selected Project ID: %04d\n", project_id);
                        printf("\nAcknowledging the order...\n");
                        printf("\nAcknowledged successfully. Returning to the menu.\n");
                        getchar();
                        return;
                    }

                    if (projects[i].project_id == project_id &&
                        tTransactions[j].tProject_id == projects[i].project_id &&
                        strcmp(tTransactions[j].tStatus, "Cancelled") == 0)
                    {
                        printf("\nYou selected Project ID: %04d\n", project_id);
                        printf("\nAcknowledging the order...\n");
                        delete_transaction(project_id);
                        delete_transaction(projects[i].project_id);
                        save_user_to_file();
                        printf("\nAcknowledged successfully. Returning to the menu.\n");
                        getchar();
                        return;
                    }

                    if (projects[i].project_id == project_id &&
                        tTransactions[j].tProject_id == projects[i].project_id &&
                        strcmp(tTransactions[j].tStatus, "Pending") == 0)
                    {
                        int cc;
                        printf("\nYou selected Project ID: %04d\n", project_id);

                        while (1)
                        {
                            printf("\nDo you want to cancel this order?(yes=1|no=0): ");
                            scanf("%d", &cc);

                            if (cc == 1)
                            {
                                delete_transaction(projects[i].project_id);

                                for (int k = 0; k < user_count; k++)
                                {
                                    if (strcmp(tTransactions[j].tSeller_name, users[k].username) == 0)
                                    {
                                        if (find_name_in_transaction(users[k].username) < 1)
                                        {
                                            strcpy(users[k].order_status, "none");
                                            strcpy(users[k].buyer_name, "none");
                                            users[k].order_amount = 0;
                                            save_user_to_file();
                                        }
                                    }
                                }

                                users[current_user_index].balance += projects[i].price;
                                strcpy(projects[i].status, "Available");
                                save_all_projects_to_file();
                                printf("\nCancelled successfully. Returning to the menu.\n");
                                getchar();
                                save_user_to_file();
                                return;
                            }

                            if (cc == 0)
                            {
                                break;
                            }
                            else
                            {
                                printf("\nInvalid! Try again.\n");
                            }
                        }
                    }
                }
            }
        }
        else if (choice == 3)
        {
            printf("\nReturning to the previous menu.\n");
            getchar();
            return;
        }
        else
        {
            printf("\nInvalid choice. Please try again.\n");
            getchar();
        }
        load_transactions();
    }
}

void save_user_to_file()
{
    // Open file in append mode instead of write mode
    FILE *file = fopen("users.txt", "w"); // Change this to "a" mode
    if (!file)
    {
        printf("Error: Unable to open 'users.txt' for writing.\n");
        return;
    }

    for (int i = 0; i < user_count; i++)
    {
        // Ensure all fields are printed, including email
        fprintf(file, "%s %s %s %s %s %.2f %.2f %s\n",
                users[i].username,
                users[i].password,
                users[i].answer_security_question1,
                users[i].answer_security_question2,
                users[i].answer_security_question3,
                users[i].carbon_credit,
                users[i].balance,
                users[i].email[0] ? users[i].email : "none");
    }

    fclose(file);
}
int filter_projects(const char *project_type, float min_carbon_credit, float max_carbon_credit,
                    float min_price, float max_price, int min_duration, int max_duration)
{
    printf("\nSearching projects...\n");
    bool found = false;
    printf("Total projects loaded: %d\n", project_count);
    for (int i = 0; i < project_count; i++)
    {
        bool matches = true;
        if (strcmp(projects[i].status, "Available") != 0)
            matches = false;
        if (strlen(project_type) > 0 && strstr(projects[i].project_type, project_type) == NULL)
        {
            matches = false;
        }
        if ((min_carbon_credit >= 0 && projects[i].carbon_credit < min_carbon_credit) ||
            (max_carbon_credit >= 0 && projects[i].carbon_credit > max_carbon_credit))
        {
            matches = false;
        }
        if ((min_price >= 0 && projects[i].price < min_price) ||
            (max_price >= 0 && projects[i].price > max_price))
        {
            matches = false;
        }
        if ((min_duration >= 0 && projects[i].project_duration < min_duration) ||
            (max_duration >= 0 && projects[i].project_duration > max_duration))
        {
            matches = false;
        }
        if (matches)
        {
            found = true;
            printf("----------------------------------------\n");
            printf("ID: %04d\n", projects[i].project_id);
            printf("Name: %s\n", projects[i].project_name);
            printf("Description: %s\n", projects[i].project_description);
            printf("Type: %s\n", projects[i].project_type);
            printf("Carbon Credit: %.2f tons of CO2\n", projects[i].carbon_credit);
            printf("Price: %.2f\n", projects[i].price);
            printf("Duration: %d years\n", projects[i].project_duration);
            printf("Impact: %s\n", projects[i].environmental_impact);
            printf("Documents: %s\n", projects[i].related_documents);
            printf("Contact: %s\n", projects[i].contact_info);
            printf("----------------------------------------\n\n");
        }
    }
    return 1;
    if (!found)
    {
        printf("No projects found that match the criteria.\n");
        getchar();
        return 0;
    }
}

int reset_password(char *username, int user_count)
{
    char new_password[MAX_PASSWORD_LENGTH], confirm_new_password[MAX_PASSWORD_LENGTH];
    int i;

    while (1)
    {
        printf("Please enter your new password (or 0 to cancel): ");
        get_password(new_password, sizeof(new_password));
        if (strcmp(new_password, "0") == 0)
        {
            printf("Password reset cancelled.\n");
            return 0;
        }

        printf("Please confirm your new password: ");
        get_password(confirm_new_password, sizeof(confirm_new_password));

        if (strcmp(new_password, confirm_new_password) != 0)
        {
            printf("Passwords do not match. Please try again.\n");
            continue;
        }
        else
        {
            break;
        }
    }

    FILE *file = fopen("users.txt", "r+");
    if (!file)
    {
        printf("Error: Unable to open users.txt for reading and writing.\n");
        return -1;
    }

    User temp_users[user_count];
    bool password_changed = false;
    for (i = 0; i < user_count; i++)
    {

        fscanf(file, "%s %s %s %s %s %f", temp_users[i].username, temp_users[i].password,
               temp_users[i].answer_security_question1, temp_users[i].answer_security_question2,
               temp_users[i].answer_security_question3, &temp_users[i].carbon_credit);

        temp_users[i].username[strcspn(temp_users[i].username, "\n")] = 0;

        if (strcmp(temp_users[i].username, username) == 0)
        {
            if (strcmp(temp_users[i].password, new_password) == 0)
            {
                printf("New password cannot be the same as the old password.\n");
                fclose(file);
                return -1;
            }
            strcpy(temp_users[i].password, new_password);
            password_changed = true;

            for (int j = 0; j < user_count; j++)
            {
                if (strcmp(users[j].username, username) == 0)
                {
                    strcpy(users[j].password, new_password);
                    break;
                }
            }
        }
    }

    if (!password_changed)
    {
        printf("Username not found.\n");
        fclose(file);
        return -1;
    }

    fclose(file);

    file = fopen("users.txt", "w");
    if (!file)
    {
        printf("Error: Unable to open users.txt for writing.\n");
        return -1;
    }

    for (i = 0; i < user_count; i++)
    {
        fprintf(file, "%s %s %s %s %s %.2f %.2f %s\n",
                users[i].username,
                users[i].password,
                users[i].answer_security_question1,
                users[i].answer_security_question2,
                users[i].answer_security_question3,
                users[i].carbon_credit,
                users[i].balance,
                users[i].email);
    }

    fclose(file);
    return 0;
}
int search_projects()
{
    char project_type[MAX_PROJECT_TYPE_LENGTH] = "";
    float min_carbon_credit = 0, max_carbon_credit = FLT_MAX;
    float min_price = 0, max_price = FLT_MAX;
    int min_duration = 0, max_duration = INT_MAX;
    int search_option;

    printf("Search Options:\n");
    printf("1. Project Type\n");
    printf("2. Carbon Credit Size (tons of CO2)\n");
    printf("3. Price\n");
    printf("4. Project Duration (years)\n");
    printf("5. Start Search\n");
    printf("6. Back to Main Menu\n");

    while (1)
    {
        printf("\nChoose the option you want to filter (1-6): ");
        scanf("%d", &search_option);
        getchar();

        if (search_option == 1)
        {

            printf("Select project type:\n");
            printf("1. Renewable Energy\n");
            printf("2. Carbon Sequestration\n");
            printf("3. Waste Management\n");
            printf("4. Forestry\n");
            printf("5. Other\n");
            printf("Choose: ");
            int type_choice;
            scanf("%d", &type_choice);
            getchar();

            if (type_choice == 1)
            {
                strcpy(project_type, "Renewable Energy");
            }
            else if (type_choice == 2)
            {
                strcpy(project_type, "Carbon Sequestration");
            }
            else if (type_choice == 3)
            {
                strcpy(project_type, "Waste Management");
            }
            else if (type_choice == 4)
            {
                strcpy(project_type, "Forestry");
            }
            else if (type_choice == 5)
            {
                printf("Enter custom project type: ");
                fgets(project_type, sizeof(project_type), stdin);
                project_type[strcspn(project_type, "\n")] = '\0';
            }
            else
            {
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }
        else if (search_option == 2)
        {

            printf("Enter minimum carbon credit (or leave blank for no limit): ");
            char min_cc_input[50];
            safe_input(min_cc_input, sizeof(min_cc_input));
            if (strlen(min_cc_input) > 0)
                min_carbon_credit = atof(min_cc_input);

            printf("Enter maximum carbon credit (or leave blank for no limit): ");
            char max_cc_input[50];
            safe_input(max_cc_input, sizeof(max_cc_input));
            if (strlen(max_cc_input) > 0)
                max_carbon_credit = atof(max_cc_input);
        }
        else if (search_option == 3)
        {

            printf("Enter minimum price (or leave blank for no limit): ");
            char min_price_input[50];
            safe_input(min_price_input, sizeof(min_price_input));
            if (strlen(min_price_input) > 0)
                min_price = atof(min_price_input);

            printf("Enter maximum price (or leave blank for no limit): ");
            char max_price_input[50];
            safe_input(max_price_input, sizeof(max_price_input));
            if (strlen(max_price_input) > 0)
                max_price = atof(max_price_input);
        }
        else if (search_option == 4)
        {

            printf("Enter minimum duration (years, or leave blank for no limit): ");
            char min_duration_input[50];
            safe_input(min_duration_input, sizeof(min_duration_input));
            if (strlen(min_duration_input) > 0)
                min_duration = atoi(min_duration_input);

            printf("Enter maximum duration (years, or leave blank for no limit): ");
            char max_duration_input[50];
            safe_input(max_duration_input, sizeof(max_duration_input));
            if (strlen(max_duration_input) > 0)
                max_duration = atoi(max_duration_input);
        }
        else if (search_option == 5)
        {
            int y;
            y = filter_projects(project_type, min_carbon_credit, max_carbon_credit, min_price, max_price, min_duration, max_duration);
            if (y == 0)
            {
                return 7;
            }
            return 0;
        }
        else if (search_option == 6)
        {

            printf("Returning to Main Menu...\n");
            return 7;
        }
        else
        {

            printf("Invalid choice. Please try again.\n");
        }
    }
}

void edit_profile()
{

    char original_username[MAX_USERNAME_LENGTH];
    strcpy(original_username, users[current_user_index].username);

    char original_password[MAX_PASSWORD_LENGTH];
    strcpy(original_password, users[current_user_index].password);

    char original_answer1[MAX_ANSWER_LENGTH];
    strcpy(original_answer1, users[current_user_index].answer_security_question1);

    char original_answer2[MAX_ANSWER_LENGTH];
    strcpy(original_answer2, users[current_user_index].answer_security_question2);

    char original_answer3[MAX_ANSWER_LENGTH];
    strcpy(original_answer3, users[current_user_index].answer_security_question3);

    float original_carbon_credit = users[current_user_index].carbon_credit;
    float original_balance = users[current_user_index].balance;

    while (1)
    {
        clear_screen();
        printf("Editing Profile\n");
        int choice;
        printf("1. Edit username: %s\n", users[current_user_index].username);
        printf("2. Edit password: ********\n");
        printf("3. Edit Security Q1: What was the name of your elementary school?\nAns: %s\n", users[current_user_index].answer_security_question1);
        printf("4. Edit Security Q2: What is the name of the hospital where you were born? \nAns: %s\n", users[current_user_index].answer_security_question2);
        printf("5. Edit Security Q3: What was the name of your first pet? \nAns: %s\n", users[current_user_index].answer_security_question3);
        printf("6. Edit Carbon credit: %.2f\n", users[current_user_index].carbon_credit);
        printf("7. Edit Balance : %.2f\n", users[current_user_index].balance);
        printf("8. Save changes\n");
        printf("9. Cancel changes\n");
        printf("Choose: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            printf("Enter new username (or 0 to cancel): ");
            safe_input(users[current_user_index].username, MAX_USERNAME_LENGTH);
            if (strcmp(users[current_user_index].username, "0") == 0)
            {

                strcpy(users[current_user_index].username, original_username);
            }
            break;
        case 2:
        {
            char new_password[MAX_PASSWORD_LENGTH];
            char confirm_password[MAX_PASSWORD_LENGTH];
            while (1)
            {
                printf("Enter new password (or 0 to cancel): ");
                get_password(new_password, MAX_PASSWORD_LENGTH);
                if (strcmp(new_password, "0") == 0)
                {

                    strcpy(users[current_user_index].password, original_password);
                    break;
                }

                printf("Confirm new password: ");
                get_password(confirm_password, MAX_PASSWORD_LENGTH);

                if (strcmp(new_password, confirm_password) == 0)
                {

                    strcpy(users[current_user_index].password, new_password);
                    printf("Password updated successfully.\n");
                    break;
                }
                else
                {

                    printf("Passwords do not match. Please try again.\n");
                }
            }
            break;
        }
        case 3:
            printf("Enter new answer for question 1 (or 0 to cancel): ");
            safe_input(users[current_user_index].answer_security_question1, MAX_ANSWER_LENGTH);
            if (strcmp(users[current_user_index].answer_security_question1, "0") == 0)
            {

                strcpy(users[current_user_index].answer_security_question1, original_answer1);
            }
            break;
        case 4:
            printf("Enter new answer for question 2 (or 0 to cancel): ");
            safe_input(users[current_user_index].answer_security_question2, MAX_ANSWER_LENGTH);
            if (strcmp(users[current_user_index].answer_security_question2, "0") == 0)
            {

                strcpy(users[current_user_index].answer_security_question2, original_answer2);
            }
            break;
        case 5:
            printf("Enter new answer for question 3 (or 0 to cancel): ");
            safe_input(users[current_user_index].answer_security_question3, MAX_ANSWER_LENGTH);
            if (strcmp(users[current_user_index].answer_security_question3, "0") == 0)
            {

                strcpy(users[current_user_index].answer_security_question3, original_answer3);
            }
            break;
        case 6:
            printf("Enter new carbon credit (current: %.2f, 0 to cancel): ", users[current_user_index].carbon_credit);
            float new_credit;
            scanf("%f", &new_credit);
            if (new_credit == 0)
            {

                users[current_user_index].carbon_credit = original_carbon_credit;
            }
            else if (users[current_user_index].carbon_credit != new_credit)
            {
                users[current_user_index].carbon_credit = new_credit;
            }
            break;
        case 7:
            printf("Enter new balance (or 0 to cancel): ");
            float new_balance;
            scanf("%f", &new_balance);
            if (new_balance == 0)
            {

                users[current_user_index].balance = original_balance;
            }
            else
            {
                users[current_user_index].balance = new_balance;
            }
            break;
        case 8:
            save_user_to_file();
            printf("Profile updated successfully!\n");
            return;
        case 9:

            strcpy(users[current_user_index].username, original_username);
            strcpy(users[current_user_index].password, original_password);
            strcpy(users[current_user_index].answer_security_question1, original_answer1);
            strcpy(users[current_user_index].answer_security_question2, original_answer2);
            strcpy(users[current_user_index].answer_security_question3, original_answer3);
            users[current_user_index].carbon_credit = original_carbon_credit;
            users[current_user_index].balance = original_balance;
            printf("Profile edit cancelled. All changes reverted.\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            getchar();
            break;
        }
    }
}

void edit_project()
{
    int type_choice;
    clear_screen();
    printf("=== Edit Project ===\n");
    bool has_projects = false;

    for (int i = 0; i < project_count; i++)
    {
        if (strcmp(projects[i].username, users[current_user_index].username) == 0)
        {
            has_projects = true;
            printf("\n=== Project ID: %04d ===\n", projects[i].project_id);
            printf("Name: %s\n", projects[i].project_name);
            printf("Description: %s\n", projects[i].project_description[0] ? projects[i].project_description : "N/A");
            printf("Type: %s\n", projects[i].project_type);
            printf("Carbon Credit: %.2f tons of CO2\n", projects[i].carbon_credit);
            printf("Price: %.2f\n", projects[i].price);
            printf("Duration: %d years\n", projects[i].project_duration);
            printf("Environmental Impact: %s\n", projects[i].environmental_impact[0] ? projects[i].environmental_impact : "N/A");
            printf("Related Documents: %s\n", projects[i].related_documents[0] ? projects[i].related_documents : "N/A");
            printf("Contact Information: %s\n", projects[i].contact_info[0] ? projects[i].contact_info : "N/A");
            printf("Status: %s\n", projects[i].status);
        }
    }

    if (!has_projects)
    {
        printf("You have no projects to edit. Press Enter to return to the previous menu.\n");
        getchar();
        return;
    }

    int project_id;
    printf("\nEnter the project ID to edit (or 0 to cancel): ");
    scanf("%d", &project_id);
    getchar();

    if (project_id == 0)
    {
        return;
    }

    // Check project edit permissions AFTER getting the project_id
    if (!can_edit_project(project_id))
    {
        getchar(); // Wait for user to press Enter
        return;
    }

    int index = -1;
    for (int i = 0; i < project_count; i++)
    {
        if (projects[i].project_id == project_id &&
            strcmp(projects[i].username, users[current_user_index].username) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Project not found or you do not have permission to edit this project.\n");
        getchar();
        return;
    }

    Project original_project = projects[index];

    while (1)
    {
        clear_screen();
        printf("=== Project Details Summary ===\n");
        printf("1. Name: %s\n", projects[index].project_name);
        printf("2. Description: %s\n", projects[index].project_description[0] ? projects[index].project_description : "N/A");
        printf("3. Type: %s\n", projects[index].project_type);
        printf("4. Carbon Credit: %.2f tons of CO2\n", projects[index].carbon_credit);
        printf("5. Price: %.2f\n", projects[index].price);
        printf("6. Duration: %d years\n", projects[index].project_duration);
        printf("7. Environmental Impact: %s\n", projects[index].environmental_impact[0] ? projects[index].environmental_impact : "N/A");
        printf("8. Related Documents: %s\n", projects[index].related_documents[0] ? projects[index].related_documents : "N/A");
        printf("9. Contact Information: %s\n", projects[index].contact_info);
        printf("10. Confirm and Save\n");
        printf("11. Cancel Editing\n");
        printf("\nChoose the detail you want to edit: ");
        int choice;
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            printf("Enter new project name: ");
            safe_input(projects[index].project_name, sizeof(projects[index].project_name));
            break;
        case 2:
            printf("Enter new project description: ");
            safe_input(projects[index].project_description, sizeof(projects[index].project_description));
            break;
        case 3:
            do
            {
                printf("Select project type [required]:\n");
                printf("1. Renewable Energy\n");
                printf("2. Carbon Sequestration\n");
                printf("3. Waste Management\n");
                printf("4. Forestry\n");
                printf("5. Other\n");
                printf("Choose: ");
                scanf("%d", &type_choice);
                getchar();

                if (type_choice == 1)
                {
                    strcpy(projects[index].project_type, "Renewable Energy");
                    break;
                }
                else if (type_choice == 2)
                {
                    strcpy(projects[index].project_type, "Carbon Sequestration");
                    break;
                }
                else if (type_choice == 3)
                {
                    strcpy(projects[index].project_type, "Waste Management");
                    break;
                }
                else if (type_choice == 4)
                {
                    strcpy(projects[index].project_type, "Forestry");
                    break;
                }
                else if (type_choice == 5)
                {
                    printf("Enter custom project type [required]: ");
                    safe_input(projects[index].project_type, sizeof(projects[index].project_type));
                    break;
                }
                else
                {
                    printf("Invalid choice. Please choose again.\n");
                    getchar();
                }

            } while (1);

        case 4:
        {
            float new_carbon_credit;
            while (1)
            {
                printf("Enter new carbon credit size (tons of CO2): ");
                scanf("%f", &new_carbon_credit);
                getchar();
                float credit_difference = new_carbon_credit - projects[index].carbon_credit;

                if (credit_difference > 0 && users[current_user_index].carbon_credit < credit_difference)
                {
                    printf("Error: You do not have enough carbon credits.\n");
                    printf("Available credits: %.2f, Needed: %.2f\n",
                           users[current_user_index].carbon_credit, credit_difference);

                    int sub_choice;
                    printf("\nOptions:\n");
                    printf("1. Add more carbon credits\n");
                    printf("2. Enter a lower carbon credit value\n");
                    printf("3. Cancel editing this field\n");
                    printf("Choose: ");
                    scanf("%d", &sub_choice);
                    getchar();

                    if (sub_choice == 1)
                    {
                        float additional_credits;
                        printf("Enter the amount of carbon credits to add: ");
                        scanf("%f", &additional_credits);
                        getchar();
                        if (additional_credits > 0)
                        {
                            users[current_user_index].carbon_credit += additional_credits;
                            printf("Credits updated. Available credits: %.2f\n", users[current_user_index].carbon_credit);
                            save_user_to_file();
                        }
                        else
                        {
                            printf("Invalid amount. No credits were added.\n");
                        }
                    }
                    else if (sub_choice == 2)
                    {
                        continue;
                    }
                    else if (sub_choice == 3)
                    {
                        printf("Cancelled updating carbon credits.\n");
                        break;
                    }
                    else
                    {
                        printf("Invalid choice. Please try again.\n");
                    }
                }
                else
                {

                    if (credit_difference < 0)
                    {
                        users[current_user_index].carbon_credit -= (-credit_difference);
                        printf("Your carbon credits have been increased by %.2f\n", -credit_difference);
                    }
                    projects[index].carbon_credit = new_carbon_credit;
                    printf("Carbon credits updated successfully.\n");
                    break;
                }
            }
            break;
        }

        case 5:
            printf("Enter new price: ");
            scanf("%f", &projects[index].price);
            getchar();
            break;
        case 6:
            printf("Enter new project duration (years): ");
            scanf("%d", &projects[index].project_duration);
            getchar();
            break;
        case 7:
            printf("Enter new environmental impact: ");
            safe_input(projects[index].environmental_impact, sizeof(projects[index].environmental_impact));
            break;
        case 8:
            printf("Enter new related documents: ");
            safe_input(projects[index].related_documents, sizeof(projects[index].related_documents));
            break;
        case 9:
            printf("Enter new contact information: ");
            safe_input(projects[index].contact_info, sizeof(projects[index].contact_info));
            break;
        case 10:
            save_all_projects_to_file();
            save_user_to_file();
            printf("Project updated successfully.\n");
            return;
        case 11:

            projects[index] = original_project;
            printf("Editing cancelled. All changes reverted.\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            getchar();
            break;
        }
    }
}

void confirm_project_details(char *project_name, char *project_description, char *project_type,
                             float *carbon_credit, float *price, int *project_duration,
                             char *environmental_impact, char *related_documents, char *contact_info)
{
    clear_screen();
    while (1)
    {
        clear_screen();
        printf("=== Project Details Summary ===\n");
        printf("1. Name: %s\n", project_name);
        printf("2. Description: %s\n", project_description[0] ? project_description : "N/A");
        printf("3. Type: %s\n", project_type);
        printf("4. Carbon Credit: %.2f tons of CO2\n", *carbon_credit);
        printf("5. Price: %.2f\n", *price);
        printf("6. Duration: %d years\n", *project_duration);
        printf("7. Environmental Impact: %s\n", environmental_impact[0] ? environmental_impact : "N/A");
        printf("8. Related Documents: %s\n", related_documents[0] ? related_documents : "N/A");
        printf("9. Contact Information: %s\n", contact_info);
        printf("10. Confirm and Save\n");
        printf("\nChoose the detail you want to edit: ");
        int choice;
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            printf("Enter new project name: ");
            safe_input(project_name, 256);
            break;
        case 2:
            printf("Enter new project description: ");
            safe_input(project_description, 512);
            break;
        case 3:
            printf("Enter new project type: ");
            safe_input(project_type, MAX_PROJECT_TYPE_LENGTH);
            break;
        case 4:
        {
            while (1)
            {
                printf("Enter new carbon credit size (tons of CO2): ");
                float new_carbon_credit;
                scanf("%f", &new_carbon_credit);
                getchar();
                float credit_difference = new_carbon_credit - *carbon_credit;
                if (credit_difference > 0 && users[current_user_index].carbon_credit < credit_difference)
                {
                    printf("Error: You do not have enough carbon credits.\n");
                    printf("Available credits: %.2f, Needed: %.2f\n",
                           users[current_user_index].carbon_credit, credit_difference);
                    int sub_choice;
                    printf("\nOptions:\n");
                    printf("1. Add more carbon credits\n");
                    printf("2. Enter a lower carbon credit value\n");
                    printf("3. Cancel editing this field\n");
                    printf("Choose: ");
                    scanf("%d", &sub_choice);
                    getchar();
                    if (sub_choice == 1)
                    {
                        float additional_credits;
                        printf("Enter the amount of carbon credits to add: ");
                        scanf("%f", &additional_credits);
                        getchar();
                        if (additional_credits > 0)
                        {
                            users[current_user_index].carbon_credit += additional_credits;
                            printf("Credits updated. Available credits: %.2f\n", users[current_user_index].carbon_credit);
                            save_user_to_file();
                        }
                        else
                        {
                            printf("Invalid amount. No credits were added.\n");
                        }
                    }
                    else if (sub_choice == 2)
                    {
                        continue;
                    }
                    else if (sub_choice == 3)
                    {
                        printf("Cancelled updating carbon credits.\n");
                        break;
                    }
                    else
                    {
                        printf("Invalid choice. Please try again.\n");
                    }
                }
                else
                {
                    users[current_user_index].carbon_credit -= credit_difference;
                    *carbon_credit = new_carbon_credit;
                    printf("Carbon credits updated successfully.\n");
                    break;
                }
            }
            break;
        }
        case 5:
            printf("Enter new price: ");
            scanf("%f", price);
            getchar();
            break;
        case 6:
            printf("Enter new project duration (years): ");
            scanf("%d", project_duration);
            getchar();
            break;
        case 7:
            printf("Enter new environmental impact: ");
            safe_input(environmental_impact, 512);
            break;
        case 8:
            printf("Enter new related documents: ");
            safe_input(related_documents, 256);
            break;
        case 9:
            printf("Enter new contact information: ");
            safe_input(contact_info, 256);
            break;
        case 10:
            printf("Project details confirmed and saved.\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
            getchar();
            break;
        }
    }
}
void get_password(char *password, int max_length)
{
#ifdef _WIN32
    char ch;
    int index = 0;
    while (1)
    {
        ch = getch();
        if (ch == '\r')
        {
            break;
        }
        else if (ch == '\b')
        {
            if (index > 0)
            {
                printf("\b \b");
                index--;
            }
        }
        else if (index < max_length - 1)
        {
            password[index++] = ch;
            printf("*");
        }
    }
    password[index] = '\0';
    printf("\n");
#else
    struct termios oldt, newt;
    int index = 0;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    while (1)
    {
        ch = getchar();
        if (ch == '\n')
        {
            break;
        }
        else if (ch == '\b' || ch == 127)
        {
            if (index > 0)
            {
                printf("\b \b");
                index--;
            }
        }
        else if (index < max_length - 1)
        {
            password[index++] = ch;
            printf("*");
        }
    }
    password[index] = '\0';
    printf("\n");
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}
void safe_input(char *buffer, int max_length)
{
    fgets(buffer, max_length, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}
void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void load_users_from_file()
{
    FILE *file = fopen("users.txt", "r");
    if (!file)
    {
        printf("Error: Unable to open 'users.txt' for reading.\n");
        return;
    }

    user_count = 0;
    user_capacity = INITIAL_CAPACITY;
    users = malloc(user_capacity * sizeof(User));

    User new_user;
    // Debug print to track loading
    printf("Loading users from file...\n");

    // Ensure reading 11 fields, including email
    while (fscanf(file, "%s %s %s %s %s %f %f %s",
                  new_user.username,
                  new_user.password,
                  new_user.answer_security_question1,
                  new_user.answer_security_question2,
                  new_user.answer_security_question3,
                  &new_user.carbon_credit,
                  &new_user.balance,
                  new_user.email) == 8)
    {

        // Resize array if needed
        if (user_count >= user_capacity)
        {
            user_capacity *= 2;
            users = realloc(users, user_capacity * sizeof(User));
        }

        users[user_count] = new_user;
        user_count++;
    }

    fclose(file);
}
int user_login()
{
    if (user_count == 0)
    {
        printf("No user data loaded. Please check users.txt or press enter to go back.\n");
        getchar();
        return -1;
    }
    char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH];
    while (1)
    {
        clear_screen();
        printf("*** Login ***\n");
        printf("Enter 'back' to return to the main menu.\n");
        printf("Username: ");
        safe_input(username, sizeof(username));
        if (strcmp(username, "back") == 0)
        {
            clear_screen();
            return -1;
        }
        printf("Type 'forget' if you forget the password.\n");
        printf("Password: ");
        get_password(password, sizeof(password));
        if (strcmp(password, "back") == 0)
        {
            clear_screen();
            return -1;
        }
        if (strcmp(password, "forget") == 0)
        {
            user_forget_password(username);
            continue;
        }
        for (int i = 0; i < user_count; i++)
        {
            if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
            {
                current_user_index = i;
                printf("Login successful! Welcome, %s.\n", username);
                return 0;
            }
        }
        printf("Invalid username or password. Please try again.\n");
        getchar();
    }
}
int user_forget_password(char *username)
{
    int choose;
    int incorrect = 0;
    char answer[MAX_ANSWER_LENGTH];
    clear_screen();
    printf("Please choose a security question\n");
    printf("1: What was the name of your elementary school?\n");
    printf("2: What is the name of the hospital where you were born?\n");
    printf("3: What was the name of your first pet?\n");
    printf("Choose: ");
    scanf("%d", &choose);
    getchar();
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(username, users[i].username) == 0)
        {
            while (incorrect < 3)
            {
                switch (choose)
                {
                case 1:
                    printf("What was the name of your elementary school?\nAns: ");
                    safe_input(answer, sizeof(answer));
                    if (strcmp(answer, users[i].answer_security_question1) == 0)
                    {
                        printf("Correct answer. Proceeding to reset password.\n");
                        reset_password(username, user_count);
                        getchar();
                        return 0;
                    }
                    break;
                case 2:
                    printf("What is the name of the hospital where you were born?\nAns: ");
                    safe_input(answer, sizeof(answer));
                    if (strcmp(answer, users[i].answer_security_question2) == 0)
                    {
                        printf("Correct answer. Proceeding to reset password.\n");
                        reset_password(username, user_count);
                        return 0;
                    }
                    break;
                case 3:
                    printf("What was the name of your first pet?\nAns: ");
                    safe_input(answer, sizeof(answer));
                    if (strcmp(answer, users[i].answer_security_question3) == 0)
                    {
                        printf("Correct answer. Proceeding to reset password.\n");
                        reset_password(username, user_count);
                        return 0;
                    }
                    break;
                default:
                    printf("Invalid choice.\n");
                    getchar();
                    return -1;
                }
                printf("Incorrect answer. Please try again.\n");
                incorrect++;
                if (incorrect == 3)
                {
                    printf("Too many incorrect attempts. Please contact support.\n");
                    return -1;
                }
            }
        }
    }
    printf("Username not found.\n");
    getchar();
    return -1;
}
int user_register()
{
    char username[MAX_USERNAME_LENGTH], password[MAX_PASSWORD_LENGTH],
        confirm_password[MAX_PASSWORD_LENGTH],
        answer1[MAX_ANSWER_LENGTH],
        answer2[MAX_ANSWER_LENGTH],
        answer3[MAX_ANSWER_LENGTH],
        email[MAX_EMAIL_LENGTH]; // New field for email
    float carbon_credit = 0.0, balance = 0.0;

    while (1)
    {
        clear_screen();
        printf("*** Register ***\n");
        printf("Enter 'back' to return to the main menu.\n");

        // Username input
        printf("Username: ");
        safe_input(username, sizeof(username));
        if (strcmp(username, "back") == 0)
            return -1;

        // Check username uniqueness
        bool username_exists = false;
        for (int i = 0; i < user_count; i++)
        {
            if (strcmp(users[i].username, username) == 0)
            {
                printf("Username already exists. Please try a different one.");
                getchar();
                username_exists = true;
                break;
            }
        }
        if (username_exists)
            continue;

        // Password input with confirmation
        while (1)
        {
            printf("Password: ");
            get_password(password, sizeof(password));
            if (strcmp(password, "back") == 0)
                return -1;

            printf("Confirm Password: ");
            get_password(confirm_password, sizeof(confirm_password));
            if (strcmp(confirm_password, "back") == 0)
                return -1;

            if (strcmp(password, confirm_password) != 0)
            {
                clear_screen();
                printf("Passwords do not match. Please try again.\n");
                continue;
            }
            break;
        }

        // Security Questions
        printf("Security Questions:\n");
        printf("Q1: What was the name of your elementary school?\nAns: ");
        safe_input(answer1, sizeof(answer1));
        printf("Q2: What is the name of the hospital where you were born? \nAns: ");
        safe_input(answer2, sizeof(answer2));
        printf("Q3: What was the name of your first pet? \nAns: ");
        safe_input(answer3, sizeof(answer3));

        // Email Input with Validation
        while (1)
        {
            printf("Email Address: ");
            safe_input(email, sizeof(email));

            // Basic email validation
            if (strchr(email, '@') == NULL || strchr(email, '.') == NULL)
            {
                printf("Invalid email address. Please try again.\n");
                continue;
            }

            break;
        }

        // Existing initial credit and balance input
        printf("Enter initial carbon credits (or press enter to set to 0): ");
        char carbon_credit_input[50];
        safe_input(carbon_credit_input, sizeof(carbon_credit_input));
        if (strlen(carbon_credit_input) > 0)
        {
            carbon_credit = atof(carbon_credit_input);
        }

        printf("Enter initial balance (or press enter to set to 0): ");
        char balance_input[50];
        safe_input(balance_input, sizeof(balance_input));
        if (strlen(balance_input) > 0)
        {
            balance = atof(balance_input);
        }

        // Confirmation and editing loop
        while (1)
        {
            clear_screen();
            printf("Confirm your registration details:\n");
            printf("1. Username: %s\n", username);
            printf("2. Password: ********\n");
            printf("3. Security Question 1 Answer: %s\n", answer1);
            printf("4. Security Question 2 Answer: %s\n", answer2);
            printf("5. Security Question 3 Answer: %s\n", answer3);
            printf("6. Email: %s\n", email);
            printf("7. Carbon Credit: %.2f\n", carbon_credit);
            printf("8. Balance: %.2f\n", balance);
            printf("\nDo you want to make any changes?\n");
            printf("Enter the number to edit, or 0 to confirm and register: ");

            int choice;
            scanf("%d", &choice);
            getchar();

            if (choice == 0)
                break;

            switch (choice)
            {
            case 1:
                printf("Enter new username: ");
                safe_input(username, sizeof(username));
                break;
            case 2:
                while (1)
                {
                    printf("Enter new password: ");
                    get_password(password, sizeof(password));
                    printf("Confirm new password: ");
                    get_password(confirm_password, sizeof(confirm_password));

                    if (strcmp(password, confirm_password) == 0)
                        break;
                    else
                    {
                        printf("Passwords do not match. Try again.\n");
                        getchar();
                    }
                }
                break;
            case 3:
                printf("Enter new answer for security question 1: ");
                safe_input(answer1, sizeof(answer1));
                break;
            case 4:
                printf("Enter new answer for security question 2: ");
                safe_input(answer2, sizeof(answer2));
                break;
            case 5:
                printf("Enter new answer for security question 3: ");
                safe_input(answer3, sizeof(answer3));
                break;
            case 6:
                printf("Enter new email: ");
                safe_input(email, sizeof(email));
                break;
            case 7:
                printf("Enter new carbon credits: ");
                scanf("%f", &carbon_credit);
                getchar();
                break;
            case 8:
                printf("Enter new balance: ");
                scanf("%f", &balance);
                getchar();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                getchar();
                break;
            }
        }

        // Resize user array if needed
        if (user_count >= user_capacity)
        {
            user_capacity *= 2;
            users = realloc(users, user_capacity * sizeof(User));
        }

        // Save new user details
        User *new_user = &users[user_count];
        strcpy(new_user->username, username);
        strcpy(new_user->password, password);
        strcpy(new_user->answer_security_question1, answer1);
        strcpy(new_user->answer_security_question2, answer2);
        strcpy(new_user->answer_security_question3, answer3);
        strcpy(new_user->email, email); // Save email
        new_user->carbon_credit = carbon_credit;
        new_user->balance = balance;
        strcpy(new_user->order_status, "");
        strcpy(new_user->buyer_name, "");
        new_user->order_amount = 0;

        user_count++;
        save_user_to_file();
        printf("Registration successful! Press Enter to continue...\n");
        getchar();
        return 0;
    }
}
int buyer_menu()
{
    char project_id_input[10];
    int search;
    clear_screen();
    printf("=== Buyer Menu ===\n");

    search = search_projects();

    while (1)
    {
        if (search == 7)
            break;
        printf("\nEnter the 4-digit project ID to purchase (or 0 to cancel): ");
        safe_input(project_id_input, sizeof(project_id_input));

        if (strcmp(project_id_input, "0") == 0)
        {
            printf("Cancelled. Returning to search menu.\n");
            return 0;
        }

        if (strlen(project_id_input) == 4 && isdigit(project_id_input[0]))
        {
            int project_id = atoi(project_id_input);
            bool project_found = false;

            for (int i = 0; i < project_count; i++)
            {
                if (projects[i].project_id == project_id)
                {
                    project_found = true;

                    if (strcmp(projects[i].username, users[current_user_index].username) == 0)
                    {

                        printf("Error: You cannot purchase your own project. Please choose another project.\n");
                        break;
                    }
                    else
                    {

                        buy_from_filtered(project_id_input);
                        getchar();
                        return 0;
                    }
                }
            }

            if (!project_found)
            {
                printf("Project ID not found. Please enter a valid project ID.\n");
            }
        }
        else
        {

            printf("Invalid ID format. Please enter a 4-digit project ID.\n");
            getchar();
        }
    }
}

void seller_menu()
{
    int choice;
    do
    {
        clear_screen();
        printf("=== Seller Menu ===\n");
        printf("1. Add New Project\n");
        printf("2. Edit Existing Project\n");
        printf("3. Delete Project\n");
        printf("4. Back to Main Menu\n");
        printf("====================\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            add_new_project();
            load_projects_from_file();
            getchar();
            break;
        case 2:
            edit_project();
            break;
        case 3:
            delete_project();
            break;
        case 4:
            printf("Returning to Main Menu...\n");
            break;
        default:
            printf("Invalid choice. Press enter to try again.\n");
            getchar();
            break;
        }
    } while (choice != 4);
}

void add_new_project()
{
    Project new_project;
    char input[256];
    char buffer[1024];
    clear_screen();
    printf("Enter project name [required]: ");
    safe_input(new_project.project_name, sizeof(new_project.project_name));
    clear_screen();
    printf("Enter project description [optional, Press Enter to skip]: ");
    if (fgets(input, sizeof(input), stdin) && input[0] != '\n')
    {
        strncpy(new_project.project_description, input, sizeof(new_project.project_description) - 1);
        new_project.project_description[strcspn(new_project.project_description, "\n")] = '\0';
    }
    else
    {
        strcpy(new_project.project_description, "none");
    }

    int type_choice;
    do
    {
        clear_screen();
        printf("Select project type [required]:\n");
        printf("1. Renewable Energy\n");
        printf("2. Carbon Sequestration\n");
        printf("3. Waste Management\n");
        printf("4. Forestry\n");
        printf("5. Other\n");
        printf("Choose: ");
        scanf("%d", &type_choice);
        getchar();

        if (type_choice == 1)
        {
            strcpy(new_project.project_type, "Renewable Energy");
            break;
        }
        else if (type_choice == 2)
        {
            strcpy(new_project.project_type, "Carbon Sequestration");
            break;
        }
        else if (type_choice == 3)
        {
            strcpy(new_project.project_type, "Waste Management");
            break;
        }
        else if (type_choice == 4)
        {
            strcpy(new_project.project_type, "Forestry");
            break;
        }
        else if (type_choice == 5)
        {
            printf("Enter custom project type [required]: ");
            safe_input(new_project.project_type, sizeof(new_project.project_type));
            break;
        }
        else
        {
            printf("Invalid choice. Please choose again.\n");
            getchar();
        }

    } while (1);

    do
    {
        clear_screen();
        printf("Enter carbon credit (tons of CO2) [required]: ");
        fgets(input, sizeof(input), stdin);
        new_project.carbon_credit = atof(input);
        if (new_project.carbon_credit > 0)
        {
            if (users[current_user_index].carbon_credit < new_project.carbon_credit)
            {
                printf("Error: You do not have enough carbon credits to list this project.\n");
                printf("Your available credits: %.2f\n", users[current_user_index].carbon_credit);
                printf("Please adjust the carbon credit value or cancel.\n");
                int choice;
                while (1)
                {
                    printf("\nOptions:\n");
                    printf("1. Enter a lower carbon credit value\n");
                    printf("2. Add more carbon credits\n");
                    printf("3. Cancel\n");
                    printf("Choose: ");
                    scanf("%d", &choice);
                    getchar();
                    if (choice == 1)
                    {
                        break;
                    }
                    else if (choice == 2)
                    {
                        float additional_credits;
                        printf("Enter the amount of carbon credits to add: ");
                        scanf("%f", &additional_credits);
                        getchar();
                        if (additional_credits > 0)
                        {
                            users[current_user_index].carbon_credit += additional_credits;
                            printf("Carbon credits updated! Your available credits: %.2f\n",
                                   users[current_user_index].carbon_credit);
                            getchar();
                            save_user_to_file();
                            break;
                        }
                        else
                        {
                            printf("Invalid amount. No credits were added.\n");
                            getchar();
                        }
                    }
                    else if (choice == 3)
                    {
                        printf("Cancelled adding project.\n");
                        return;
                    }
                    else
                    {
                        printf("Invalid choice. Please try again.\n");
                        getchar();
                    }
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            printf("Invalid input. Please enter a positive number.\n");
            getchar();
        }
    } while (true);

    do
    {
        clear_screen();
        printf("Enter price [required]: ");
        fgets(input, sizeof(input), stdin);
        new_project.price = atof(input);
        if (new_project.price > 0)
            break;
        printf("Invalid input. Please enter a positive number.\n");
        getchar();
    } while (true);

    do
    {
        clear_screen();
        printf("Enter project duration (years) [required]: ");
        fgets(input, sizeof(input), stdin);
        new_project.project_duration = atoi(input);
        if (new_project.project_duration > 0)
        {
            break;
        }
        else
        {
            printf("Invalid input. Please enter a positive number greater than 0.\n");
            getchar();
        }
    } while (true);
    clear_screen();
    printf("Enter environmental impact [optional, Press Enter to skip]: ");
    if (fgets(input, sizeof(input), stdin) && input[0] != '\n')
    {
        strncpy(new_project.environmental_impact, input, sizeof(new_project.environmental_impact) - 1);
        new_project.environmental_impact[strcspn(new_project.environmental_impact, "\n")] = '\0';
    }
    else
    {
        strcpy(new_project.environmental_impact, "none");
    }

    do
    {
        clear_screen();
        printf("Enter related documents [required]: ");
        safe_input(new_project.related_documents, sizeof(new_project.related_documents));
        if (strlen(new_project.related_documents) > 0)
            break;
        printf("Related documents are required. Please enter a valid value.\n");
        getchar();
    } while (true);

    do
    {
        clear_screen();
        if (current_user_index == -1)
        {
            printf("Error: No user is logged in.\n");
            return;
        }

        if (strlen(users[current_user_index].email) == 0)
        {
            printf("Error: No email associated with your account.\n");
            return;
        }

        strcpy(new_project.contact_info, users[current_user_index].email);
        printf("Using your email: %s\n", new_project.contact_info);

        break; // ออกจากลูป
    } while (false); // ทำแค่ครั้งเดียว

    float old_carbon_credit = users[current_user_index].carbon_credit;

    save_new_project(&new_project);

    if (users[current_user_index].carbon_credit != old_carbon_credit - new_project.carbon_credit)
    {
        users[current_user_index].carbon_credit -= new_project.carbon_credit;
        save_user_to_file();
        printf("Project saving. \n");
        return;
    }

    save_all_projects_to_file();
    save_user_to_file();
}

void delete_project()
{
    printf("=== Your Projects ===\n");
    bool has_projects = false;
    for (int i = 0; i < project_count; i++)
    {
        if (strcmp(projects[i].username, users[current_user_index].username) == 0)
        {
            has_projects = true;
            printf("\n===========================\n");
            printf("Project ID: %04d \n", projects[i].project_id);
            printf("Name: %s\n", projects[i].project_name);
            printf("Description: %s\n", projects[i].project_description[0] ? projects[i].project_description : "N/A");
            printf("Type: %s\n", projects[i].project_type);
            printf("Carbon Credit: %.2f tons of CO2\n", projects[i].carbon_credit);
            printf("Price: %.2f\n", projects[i].price);
            printf("Duration: %d years\n", projects[i].project_duration);
            printf("Environmental Impact: %s\n", projects[i].environmental_impact[0] ? projects[i].environmental_impact : "N/A");
            printf("Related Documents: %s\n", projects[i].related_documents[0] ? projects[i].related_documents : "N/A");
            printf("Contact Information: %s\n", projects[i].contact_info[0] ? projects[i].contact_info : "N/A");
            printf("Status: %s\n", projects[i].status);
            printf("\n===========================\n");
        }
    }
    if (!has_projects)
    {
        printf("You have no projects to delete. Press Enter to return to the previous menu.\n");

        return;
    }
    int project_id;
    project_id = delete_project_selection(project_id, users[current_user_index].username);
    if (project_id == 0)
    {
        return;
    }
    int found = -1;
    for (int i = 0; i < project_count; i++)
    {

        if (projects[i].project_id == project_id && strcmp(projects[i].username, users[current_user_index].username) == 0)
        {
            found = i;
            break;
        }
    }
    if (found != -1)
    {
        float returned_credit = projects[found].carbon_credit;
        users[current_user_index].carbon_credit += returned_credit;
        printf("Carbon credit of %.2f tons of CO2 has been refunded to your account.\n", returned_credit);
        for (int i = found; i < project_count - 1; i++)
        {
            projects[i] = projects[i + 1];
        }
        project_count--;
        save_all_projects_to_file();
        save_user_to_file();
        printf("Project deleted successfully.\n");
        getchar();
    }
}

void load_projects_from_file()
{
    FILE *file = fopen("projects.txt", "r");
    if (!file)
    {
        printf("Error: 'projects.txt' not found.\n");
        return;
    }
    Project temp;
    project_count = 0;
    while (1)
    {
        int result = fscanf(file, "\"%d\",\"%255[^\"]\",\"%511[^\"]\",\"%255[^\"]\",%f,%f,%d,\"%511[^\"]\",\"%255[^\"]\",\"%255[^\"]\",\"%255[^\"]\",\"%50[^\"]\"",
                            &temp.project_id, temp.project_name, temp.project_description, temp.project_type,
                            &temp.carbon_credit, &temp.price, &temp.project_duration, temp.environmental_impact,
                            temp.related_documents, temp.contact_info, temp.username, temp.status);
        if (result == EOF)
        {
            break;
        }
        if (result != 12)
        {
            char buffer[1024];
            fgets(buffer, sizeof(buffer), file);
            continue;
        }
        if (project_count >= project_capacity)
        {
            project_capacity *= 2;
            projects = realloc(projects, project_capacity * sizeof(Project));
            if (!projects)
            {
                printf("Error: Memory allocation failed for projects.\n");
                fclose(file);
                return;
            }
        }
        projects[project_count++] = temp;
    }
    fclose(file);
}

void buy_from_filtered(char project_id_input[10])
{
    while (1)
    {
        if (strlen(project_id_input) == 0 || !isdigit(project_id_input[0]))
        {
            printf("Invalid input. Please enter a valid 4-digit project ID (or 0 to cancel)\n");
            safe_input(project_id_input, sizeof(project_id_input));
            continue;
        }
        if (strcmp(project_id_input, "0") == 0)
        {
            printf("Cancelled. Returning to the search menu.\n");
            return;
        }
        if (strlen(project_id_input) != 4)
        {
            printf("Invalid ID format. Please enter a 4-digit project ID.\n");
            safe_input(project_id_input, sizeof(project_id_input));
            continue;
        }
        bool project_found = false;
        for (int i = 0; i < project_count; i++)
        {
            if (projects[i].project_id == atoi(project_id_input))
            {
                project_found = true;
                if (strcmp(projects[i].status, "Available") != 0)
                {
                    printf("This project is not available for purchase.\n");

                    return;
                }
                User *seller = NULL;
                for (int j = 0; j < user_count; j++)
                {
                    if (strcmp(users[j].username, projects[i].username) == 0)
                    {
                        seller = &users[j];
                        break;
                    }
                }
                if (!seller)
                {
                    printf("Error: Seller not found. Transaction cannot proceed.\n");
                    return;
                }
                if (users[current_user_index].balance >= projects[i].price)
                {

                    printf("\n--- Project Details ---\n");
                    printf("Project ID: %04d\n", projects[i].project_id);
                    printf("Project Type: %s\n", projects[i].project_type);
                    printf("Price: %.2f\n", projects[i].price);
                    printf("Contact: %s\n", projects[i].contact_info);
                    printf("Description: %s\n", projects[i].related_documents);

                    printf("\nYour current balance: %.2f\n", users[current_user_index].balance);

                    char confirm;
                    while (1)
                    {
                        printf("\nDo you want to proceed with the purchase? (no=0|yes=1): ");
                        scanf(" %d", &confirm);
                        if (confirm == 1)
                        {

                            strcpy(projects[i].status, "Pending");
                            strcpy(seller->order_status, "Pending");
                            strcpy(seller->buyer_name, users[current_user_index].username); // change
                            seller->order_amount = projects[i].price;                       // change
                            strcpy(users[current_user_index].order_status, "Pending");
                            strcpy(users[current_user_index].buyer_name, seller->username); // change
                            users[current_user_index].order_amount = projects[i].price;     // change
                            users[current_user_index].balance -= projects[i].price;
                            transaction(projects[i].project_id, projects[i].project_name, projects[i].username, users[current_user_index].username, projects[i].status);

                            printf("\nPurchase request submitted successfully! The project is now pending seller confirmation.\n");
                            getchar();
                            save_all_projects_to_file();
                            save_user_to_file();
                            return;
                        }
                        else if (confirm == 0)
                        {
                            printf("\nPurchase cancelled.\n");
                            getchar();
                            return;
                        }
                        else
                        {
                            printf("\nInvalid! Try again.\n");
                            getchar();
                        }
                    }
                }
                else
                {
                    printf("Insufficient balance to buy this project. Your balance: %.2f, Price: %.2f\n",
                           users[current_user_index].balance, projects[i].price);
                    int choice;
                    printf("\nOptions:\n");
                    printf("1. Add more balance\n");
                    printf("2. Cancel\n");
                    printf("Choose: ");
                    scanf("%d", &choice);
                    getchar();
                    if (choice == 1)
                    {
                        float additional_balance;
                        printf("Enter the amount of balance to add: ");
                        scanf("%f", &additional_balance);
                        getchar();
                        if (additional_balance > 0)
                        {
                            users[current_user_index].balance += additional_balance;
                            printf("Balance updated! Your available balance: %.2f\n",
                                   users[current_user_index].balance);
                            save_user_to_file();
                            char confirm;
                            while (1)
                            {
                                printf("\nDo you want to proceed with the purchase? (no=0|yes=1): ");
                                scanf(" %d", &confirm);
                                if (confirm == 1)
                                {

                                    strcpy(projects[i].status, "Pending");
                                    strcpy(seller->order_status, "Pending");
                                    strcpy(seller->buyer_name, users[current_user_index].username); // change
                                    seller->order_amount = projects[i].price;                       // change
                                    strcpy(users[current_user_index].order_status, "Pending");
                                    strcpy(users[current_user_index].buyer_name, seller->username); // change
                                    users[current_user_index].order_amount = projects[i].price;     // change
                                    users[current_user_index].balance -= projects[i].price;
                                    transaction(projects[i].project_id, projects[i].project_name, projects[i].username, users[current_user_index].username, projects[i].status);
                                    printf("\nPurchase request submitted successfully! The project is now pending seller confirmation.\n");
                                    getchar();
                                    save_all_projects_to_file();
                                    save_user_to_file();
                                    return;
                                }
                                else if (confirm == 0)
                                {
                                    printf("\nPurchase cancelled.\n");
                                    getchar();
                                    return;
                                }
                                else
                                {
                                    printf("\nInvalid! Try again.\n");
                                    getchar();
                                }
                            }
                        }
                        else
                        {
                            printf("Invalid amount. No balance was added.\n");
                            getchar();
                        }
                    }
                    if (choice == 2)
                    {
                        printf("Returning to the previous menu.\n");
                        return;
                    }
                    else
                    {
                        printf("Invalid choice. Returning to the previous menu.\n");
                        getchar();
                        return;
                    }
                }
            }
        }
        if (!project_found)
        {
            printf("Project ID %s not found. Please try again.\n", project_id_input);
            safe_input(project_id_input, sizeof(project_id_input));
        }
    }
}
void print_file_content(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: File '%s' not found.\n", filename);
        return;
    }
    char line[1024];
    printf("Contents of '%s':\n", filename);
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    fclose(file);
}
bool can_edit_project(int project_id)
{
    // Find the project with the given project_id
    for (int i = 0; i < project_count; i++)
    {
        // Check if the project matches the given project_id
        if (projects[i].project_id == project_id)
        {
            // Check if the current user is the project owner
            if (strcmp(projects[i].username, users[current_user_index].username) == 0)
            {
                // Check if the project status is "Available"
                if (strcmp(projects[i].status, "Available") == 0)
                {
                    return true; // User can edit the project
                }
                else
                {
                    printf("Project cannot be edited. Current status: %s\n", projects[i].status);
                    return false;
                }
            }
            else
            {
                printf("You do not have permission to edit this project.\n");
                return false;
            }
        }
    }

    // Project not found
    printf("Project not found.\n");
    return false;
}
int main()
{
    users = malloc(INITIAL_CAPACITY * sizeof(User));
    projects = malloc(INITIAL_CAPACITY * sizeof(Project));
    user_capacity = project_capacity = INITIAL_CAPACITY;
    print_file_content("projects.txt");
    load_users_from_file();
    load_projects_from_file();
    int main_choice;
    do
    {
        clear_screen();
        printf("=== Carbon Credit Trading System ===\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("==============================\n");
        printf("Choose an option: ");
        if (scanf("%d", &main_choice) != 1)
        {
            while (getchar() != '\n')
                ;
            clear_screen();
            printf("Invalid input! Please enter a valid number.\n");
            continue;
        }
        getchar();
        switch (main_choice)
        {
        case 1:
            clear_screen();
            if (user_login() == 0)
            {
                main_menu();
            }
            break;
        case 2:
            clear_screen();
            user_register();
            break;
        case 3:
            printf("Exiting the Carbon Credit Trading System. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 3.\n");
            getchar();
            break;
        }
    } while (main_choice != 3);
    free(users);
    free(projects);
    if (filtered_users)
    {
        free(filtered_users);
    }
    return 0;
}
