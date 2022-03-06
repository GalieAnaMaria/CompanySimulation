#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"
#include "list.h"

/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */

Tree search(Tree currentManager, char *manager_name){

    List list = createList();  
    constructList(list,currentManager);
    List temp = list;

    while(temp !=NULL){
        TreeNode *currentNode = temp->node;
        if(strcmp(currentNode->name,manager_name) == 0){

            freeList(list);
            return currentNode;
        }

        temp = temp->next;
    }

    freeList(list);
    return NULL;
}

void sortEmployees(TreeNode **team, int employeesNr){

    Tree temp;
    int stop = 0;

    while(stop == 0){
        stop = 1;

        for(int i = 0; i<employeesNr-1; i++){

            int n1len = strlen(team[i]->name);
            int n2len = strlen(team[i+1]->name);

            char *n2 = team[i+1]->name;
            char *n1 = team[i]->name;

            if(strcmp(n1,n2) > 0){

                temp = team[i];
                team[i] = team[i+1];
                team[i+1] = temp;

                stop = 0;
            }       
        }
    }

}

Tree hire(Tree tree, char *employee_name, char *manager_name){

    if(tree == NULL && manager_name == NULL){
        
        tree = calloc(1, sizeof(TreeNode));
        tree->name = employee_name;
        tree->direct_employees_no = 0;
        tree->team = calloc(20, sizeof(Tree));
        tree->manager = NULL;

        return tree;
    }
    
    TreeNode* node = calloc(1, sizeof(TreeNode));
	node->name = employee_name;
	node->direct_employees_no = 0;
    node->team = calloc(20, sizeof(Tree));
    node->manager = NULL;

    Tree managerNode = tree;

    if(managerNode->name == manager_name){

        int employeesNr = managerNode->direct_employees_no;

        managerNode->team[employeesNr] = node;
        node->manager = managerNode;
        managerNode->direct_employees_no++;

        sortEmployees(managerNode->team, managerNode->direct_employees_no);

        return tree;
    }

    managerNode = search(managerNode, manager_name);
        
    int employeesNr = managerNode->direct_employees_no;    

    managerNode->team[employeesNr] = node;
    node->manager = managerNode;
    managerNode->direct_employees_no++;

    sortEmployees(managerNode->team, managerNode->direct_employees_no);

    return tree;
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */

void insertUnfiredEmployee(TreeNode **team, List head, 
    int newManagerTeamNr, char *employee_name){

   for(int i=0; i<newManagerTeamNr; i++){

        if(strcmp(team[i]->name, employee_name) != 0){

            insertNode(head, team[i]);
            
        }
        team[i] = NULL;     
    }
    
}

void moveSubordinates(TreeNode* employeeToFire, TreeNode* managerEmployee, char* employee_name){

    int managerTeamNr = managerEmployee->direct_employees_no;


    for(int i=0; i<employeeToFire->direct_employees_no; i++){
      
        employeeToFire->team[i]->manager = managerEmployee;
        managerEmployee->team[managerTeamNr + i] = employeeToFire->team[i];

        employeeToFire->team[i] = NULL;
        managerEmployee->direct_employees_no++;
    }

    int newManagerTeamNr = managerEmployee->direct_employees_no;
    List listTeamAfter = createList();  

    insertUnfiredEmployee(managerEmployee->team, listTeamAfter,
        newManagerTeamNr, employee_name);


    newManagerTeamNr--;
    managerEmployee->direct_employees_no--;
    employeeToFire->direct_employees_no = 0;

    List temp = listTeamAfter;

    for(int i=0; i<newManagerTeamNr; i++){

        managerEmployee->team[i] = temp->node;
        temp = temp->next;
    }

    freeList(listTeamAfter);
}

Tree fire(Tree tree, char *employee_name){

    Tree employeeToFire = tree;

    employeeToFire = search(employeeToFire, employee_name);

    if(employeeToFire == NULL){
        return tree;
    }

    if(employeeToFire == tree){
        return tree;
    }

    Tree managerEmployee = employeeToFire->manager;
    
    moveSubordinates(employeeToFire, managerEmployee, employee_name);

    free(employeeToFire->team);
    free(employeeToFire);

    sortEmployees(managerEmployee->team, managerEmployee->direct_employees_no);

    return tree;
 
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */

Tree promote(Tree tree, char *employee_name){

    Tree employeeToMove;
    employeeToMove = search(tree, employee_name); 

    if(employeeToMove == NULL){
        return tree;
    }

    if(employeeToMove == tree){
        return tree;
    }

    if(employeeToMove->manager == tree){
        return tree;
    }

    Tree oldManager = employeeToMove->manager;
    int oldManagerTeamNr = oldManager->direct_employees_no;

    Tree managerToMoveAt = oldManager->manager;
    int managerToMoveAtTeamNr = managerToMoveAt->direct_employees_no;

    
    moveSubordinates(employeeToMove, oldManager, employee_name);
    sortEmployees(oldManager->team, oldManager->direct_employees_no);

    employeeToMove->manager = managerToMoveAt;
    managerToMoveAt->team[managerToMoveAtTeamNr] = employeeToMove;
    managerToMoveAt->direct_employees_no++;

    sortEmployees(managerToMoveAt->team, managerToMoveAt->direct_employees_no);

    return tree;        
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name){

    Tree employeeToMove;
    employeeToMove = search(tree, employee_name); 

    if(employeeToMove == NULL){
        return tree;
    }

    if(employeeToMove == tree){
        return tree;
    }

    if(employeeToMove->manager == tree && strcmp(tree->name,new_manager_name) == 0){
        return tree;
    }

    Tree oldManager = employeeToMove->manager;
    int oldManagerTeamNr = oldManager->direct_employees_no;

    Tree managerToMoveAt = search(tree, new_manager_name); 
    int managerToMoveAtTeamNr = managerToMoveAt->direct_employees_no;

    moveSubordinates(employeeToMove, oldManager, employee_name);
    sortEmployees(oldManager->team, oldManager->direct_employees_no);

    employeeToMove->manager = managerToMoveAt;
    managerToMoveAt->team[managerToMoveAtTeamNr] = employeeToMove;
    managerToMoveAt->direct_employees_no++;

    sortEmployees(managerToMoveAt->team, managerToMoveAt->direct_employees_no);

    return tree; 

}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name){

    Tree employeeManagerTeam;
    employeeManagerTeam = search(tree, employee_name); 

    if(employeeManagerTeam == NULL){
        return tree;
    }

    if(employeeManagerTeam == tree){
        return tree;
    }

    if(employeeManagerTeam->direct_employees_no == 0){
        tree = move_employee(tree, employee_name, new_manager_name);
        return tree;
    }

    Tree newTeamManager;
    newTeamManager = search(tree, new_manager_name);

    Tree oldManagerOfTheTeam = employeeManagerTeam->manager;
    List listOldTeamAfter = createList();  

    insertUnfiredEmployee(oldManagerOfTheTeam->team, listOldTeamAfter,
        oldManagerOfTheTeam->direct_employees_no, employee_name);

    List temp = listOldTeamAfter;
    
    oldManagerOfTheTeam->direct_employees_no--;

    for(int i=0; i<oldManagerOfTheTeam->direct_employees_no; i++){

        oldManagerOfTheTeam->team[i] = temp->node;
        temp = temp->next;
    }

    sortEmployees(oldManagerOfTheTeam->team, oldManagerOfTheTeam->direct_employees_no);

    employeeManagerTeam->manager = newTeamManager;
    newTeamManager->team[newTeamManager->direct_employees_no] = employeeManagerTeam;
    newTeamManager->direct_employees_no++;

    sortEmployees(newTeamManager->team, newTeamManager->direct_employees_no);

    return tree;
    
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */
void destroy_tree(Tree tree);

Tree fire_team(Tree tree, char *employee_name){

    Tree employeeManagerTeam;
    employeeManagerTeam = search(tree, employee_name); 

    if(employeeManagerTeam == NULL){
        return tree;
    }

    if(employeeManagerTeam == tree){
        return tree;
    }

    Tree oldManagerOfTheTeam = employeeManagerTeam->manager;
    List listOldTeamAfter = createList();  

    insertUnfiredEmployee(oldManagerOfTheTeam->team, listOldTeamAfter,
        oldManagerOfTheTeam->direct_employees_no, employee_name);

    List temp = listOldTeamAfter;
    
    oldManagerOfTheTeam->direct_employees_no--;

    for(int i=0; i<oldManagerOfTheTeam->direct_employees_no; i++){

        oldManagerOfTheTeam->team[i] = temp->node;
        temp = temp->next;
    }

    sortEmployees(oldManagerOfTheTeam->team, oldManagerOfTheTeam->direct_employees_no);

    destroy_tree(employeeManagerTeam);

    return tree;
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */
void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) {
    return;
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */
void get_employees_by_level(FILE *f, Tree tree, int level) {
    return;
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void get_best_manager(FILE *f, Tree tree) {
    return;
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name) {
    return NULL;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void preorder_traversal(FILE *f, Tree tree){
 
    if(tree == NULL)return;

    List listEmployee = createList();

    constructList(listEmployee, tree);

    List temp = listEmployee;

    while(temp != NULL){

        TreeNode* currentManager = temp->node;

        fprintf(f,"%s", currentManager->name);

        if(currentManager->manager != NULL){
            fprintf(f,"-%s",currentManager->manager->name);
        }

        fprintf(f," ");
        temp = temp->next;
    }

    fprintf(f,"\n");
    freeList(listEmployee);

    return;
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree){

    List list = createList();
    constructList(list, tree);

    List temp = list;

    while(temp != NULL){
        TreeNode* currentManager = temp->node;

        for(int i=0; i<=(currentManager->direct_employees_no - 1); i++)
            currentManager->team[i] = NULL;

        free(currentManager->team);
        free(currentManager);
        temp = temp->next;
    }

    freeList(list);

    return;
}