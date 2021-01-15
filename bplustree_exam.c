#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define TEST { 10,1,3,63,82,6,31,8,2,16,11,77,96,14,92,21,47,23,24,78,26,97,15,4,30,69,37,38,76,90,17,87,53,44,45,46,9,41,54,43,22,84,58,39,65,28,42,59,99,70,71,72,29,74,73,68,13,60,79,80,81,85,83,64,94,86,66,88,93,40,91,62,25,20,36,95,19,52,55,100 }
#define MIN_DEGREE 2
#define MAX_KEY (MIN_DEGREE*2 - 1)

typedef struct _node {
    bool is_leaf;
    int key[MAX_KEY + 1], key_count;
    struct _node *pointer[MAX_KEY + 2], *parent, *left, *right;
} node;

node *node_create();
void b_plus_tree_create(node **root);
void b_plus_tree_insert(node **root, int k, int value);
void b_plus_tree_delete(node *sub_root, node **root, int k);
void b_plus_tree_search(node* sub_root, int k);
void b_plus_tree_linear_search(node* sub_root, int k, int n);
void node_insert(node *sub_root, int k, int v);
void node_split(node *parent, int child_index);
void node_delete(node *sub_root, int k);
void move_key_right_to_left(node *left_child, node *right_child, int *parent_key);
void move_key_left_to_right(node *left_child, node *right_child, int *parent_key);
void bind_node(node *parent, node *left_child, node *right_child, int index);
void bind_leaf_node(node *parent, node *left_child, node *right_child, int index);
void display(node *cur_node, int blanks);
void test_case(node **root, int size);
int SUCC(node *succ_child);
int PRED(node *pred_child);
void print_for_exam(node* cur);
void test_case(node **root, int size);

// 테스트
int main() {
    node *root;
    b_plus_tree_create(&root);
    int arr[80] = TEST;


    // TEST 1 CASE
    b_plus_tree_insert(&root, 4, 4 * 1000);
    b_plus_tree_insert(&root, 1, 1 * 1000);
    b_plus_tree_insert(&root, 3, 3 * 1000);
    b_plus_tree_insert(&root, 2, 2 * 1000);

    b_plus_tree_delete(root, &root, 4);
    b_plus_tree_delete(root, &root, 2);
    b_plus_tree_delete(root, &root, 3);

    printf("---- TEST1 ----");
    print_for_exam(root);

    // TEST 2 CASE
    for (int i = 2; i <= 100; i++) {
        b_plus_tree_insert(&root, i, i * 1000);
    }
    for (int i = 50; i <= 70; i++) {
        b_plus_tree_delete(root, &root, i);
    }

    printf("\n\n\n\n\n\n");
    printf("---- TEST2 ----");
    print_for_exam(root);

    // TEST3 CASE
    for (int i = 50; i <= 70; i++) {
        b_plus_tree_insert(&root, i, i * 1000);
    }

    for (int i = 0; i <80; i++) {
        b_plus_tree_delete(root, &root, arr[i]);
    }

    printf("\n\n\n\n\n\n");
    printf("---- TEST3 ----");
    print_for_exam(root);

    printf("프로그램이 정상적으로 종료 되었음.");
    return 0;
}

void test_case(node **root, int size) {
    int* out_arr = (int*)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        out_arr[i] = i;
    }
    for (int i = 0; i < size; i++)
    {
        int j = i + rand() / (RAND_MAX / (size - i) + 1);
        int t = out_arr[j];
        out_arr[j] = out_arr[i];
        out_arr[i] = t;
    }
    for (int i = 0; i < size; i++) {
        int r = out_arr[i];
        b_plus_tree_insert(root, r, r * 3);
    }
     for (int i = 0; i < size; i++) {
         int r = out_arr[i];
         b_plus_tree_delete(*root, root, r);
     }
}


// 출력(DFS)
void print_for_exam(node* cur) {
    if (cur->is_leaf) {
        for (int i = 1; i <= cur->key_count; i++) {
            int* value = (int *)cur->pointer[i];
            printf("[%5d, %5d]\n", cur->key[i], *value);
        }
    } else {
        for (int i = 1; i <= cur->key_count; i++) {
            print_for_exam(cur->pointer[i]);
            printf("[%5d]\n", cur->key[i]);
        }
        print_for_exam(cur->pointer[cur->key_count + 1]);
    }
}


node *node_create() {
    node *new_node = (node *)malloc(sizeof(node));
    if (new_node == NULL) {
        perror("Record creation.");
        exit(EXIT_FAILURE);
    }
    new_node->is_leaf = true;
    new_node->key_count = 0;
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

void b_plus_tree_create(node **root) {
    node *new_node = node_create();
    *root = new_node;
}

void b_plus_tree_search(node* sub_root, int k) {
    int i = 1;
    while (i <= sub_root->key_count && k > sub_root->key[i]) {
        i = i + 1;
    }
    if (!sub_root->is_leaf){
        if (k == sub_root->key[i]) {
            b_plus_tree_search(sub_root->pointer[i + 1], k);
        } else {
            b_plus_tree_search(sub_root->pointer[i], k);
        }
    } else if (sub_root->is_leaf) {
        if (k == sub_root->key[i]) {
            printf("success\n");
            int* value_point = (int*)sub_root->pointer[i];
            printf("key=%d, value=%d \n", sub_root->key[i], *value_point);
            // printf("%p")
        } else {
            printf("fail\n");
        }
    }
}

void b_plus_tree_linear_search(node* sub_root, int k, int n) {
    int i = 1;
    while (i <= sub_root->key_count && k > sub_root->key[i]) {
        i = i + 1;
    }
    if (!sub_root->is_leaf){
        if (k == sub_root->key[i]) {
            b_plus_tree_linear_search(sub_root->pointer[i + 1], k, n);
        } else {
            b_plus_tree_linear_search(sub_root->pointer[i], k, n);
        }
    } else if (sub_root->is_leaf) {
        if (k == sub_root->key[i]) {
            int cnt = 0;
            for (int j = i; cnt < n && j <= sub_root->key_count; j ++, cnt ++){
                int* value_point = (int*)sub_root->pointer[j];
                printf("%d ", *value_point);
            }
            sub_root = sub_root->right;
            while(1) {
                if (sub_root == NULL){
                    break;
                }
                for (int j = 1; cnt < n && j <= sub_root->key_count; j ++, cnt ++){
                    int* value_point = (int*)sub_root->pointer[j];
                    printf("%d ", *value_point);
                }

                if (cnt == n){
                    break;
                }
                sub_root = sub_root->right;
            }
            printf("\n");
        } else {
            printf("%d not in tree\n", k);
        }
    }
}


void b_plus_tree_insert(node **root, int k, int v) {
    node *curr_root = *root;
    if((*root)->key_count == MAX_KEY) {
        node *new_root = node_create();
        *root = new_root;
        new_root->is_leaf = false;
        new_root->pointer[1] = curr_root;
        curr_root->parent = new_root;
        node_split(new_root, 1);
        node_insert(new_root, k, v);
    }
    else {
        node_insert(curr_root, k, v);
    }
}

void node_split(node *parent, int child_index) {
    node *right_child = node_create();
    node *left_child = parent->pointer[child_index];
    right_child->is_leaf = left_child -> is_leaf;
    right_child->parent = parent;
    // 최종에러는 여기였다 시작점에도 +1 해줘야하는데 끝점에만 해주고있었다 ㅠ
    for (int i = parent->key_count + 1; i >= child_index + 1; i--) {
        parent->pointer[i + 1] = parent->pointer[i];
    }
    for (int i = parent->key_count; i >= child_index; i--) {
        parent->key[i + 1] = parent->key[i];
    }
    parent->pointer[child_index + 1] = right_child;
    parent->key[child_index] = left_child->key[MIN_DEGREE];
    parent->key_count += 1;

    if (right_child->is_leaf) {
        for (int i = 1; i <= MIN_DEGREE - 1; i ++) {
            right_child->key[i] = left_child->key[i + MIN_DEGREE];
            right_child->pointer[i] = left_child->pointer[i + MIN_DEGREE];
        }
        right_child->key_count = MIN_DEGREE - 1;
        left_child->key_count = MIN_DEGREE;
        left_child->right = right_child;
        right_child->left = left_child;
        right_child->right = left_child->right;
    }
    else {
        for (int i = 1; i <= MIN_DEGREE - 1; i ++) {
            right_child->key[i] = left_child->key[i + MIN_DEGREE];
        }
        for (int i = 1; i <= MIN_DEGREE; i++) {
            right_child->pointer[i] = left_child->pointer[i + MIN_DEGREE];
        }
        right_child->key_count = MIN_DEGREE - 1;
        left_child->key_count = MIN_DEGREE - 1;
    }
}

void node_insert(node *sub_root, int k, int v) {
    int *value = (int *)malloc(sizeof(int));
    *value = v;
    int i = sub_root->key_count;
    if (sub_root->is_leaf){
        while (i >= 1 && k < sub_root->key[i]) {
            sub_root->key[i + 1] = sub_root->key[i];
            sub_root->pointer[i + 1] = sub_root->pointer[i];
            i -= 1;
        }
        sub_root->key[i + 1] = k;
        sub_root->pointer[i + 1] = (void *)value;
        sub_root->key_count += 1;
    }
    else {
        while (i >= 1 && k < sub_root->key[i]) {
            i -= 1;
        }
        i += 1;
        if (sub_root->pointer[i]->key_count == MAX_KEY) {
            node_split(sub_root, i);
            if (k > sub_root->key[i]) {
                i += 1;
            }
        }
        node_insert(sub_root->pointer[i], k, v);
    }
}

// 내부노드 키 안지우기
void b_plus_tree_delete(node *sub_root, node **root, int k) {
    if ((*root)->key_count == 0) {
        if ((*root)->is_leaf) {
            printf("tree is empty\n");
            return;
        }
    }
    node_delete(sub_root, k);
    if ((*root)->key_count == 0) {
        if ((*root)->is_leaf) {
            printf("tree is empty\n");
            free(*root);
            b_plus_tree_create(root);
            return;
        }
        else {
            node *old_root = *root;
            *root = (*root)->pointer[1]; // 병합 후에 루트는 비고, 자식 하나
            free(old_root);
            return;
        }
    }
}

void node_delete(node *sub_root, int k) {
    if (sub_root->is_leaf){ // 리프일 때
        int original_key_count = sub_root->key_count;
        for (int i = 1; i <= sub_root->key_count; i++) {
            if (sub_root->key[i] == k){
                // free(sub_root->pointer[i]);
                for (int j = i; j < sub_root->key_count; j ++) {
                    sub_root->key[j] = sub_root->key[j + 1];
                    sub_root->pointer[j] = sub_root->pointer[j + 1];
                }
                sub_root->key_count -= 1;
                break;
            }
        }

        return;
    }

    // 리프 아닐 때
    int i = 1;
    // 인덱스를 찾는데, 같을 때까진 왼쪽, 넘어갔을 경우 오른쪽으로 간다
    while(sub_root->key[i] < k && i <= sub_root->key_count) {
        i += 1;
    }
    // 오른쪽 맨끝을 넘어갔다면 (맨 오른쪽 노드)
    if (i == sub_root->key_count + 1) {
        node *left_child = sub_root->pointer[i - 1];
        node *right_child = sub_root->pointer[i];
        if (right_child->key_count >= MIN_DEGREE) {
            node_delete(right_child, k);
            return;
        }
        if (left_child->key_count >= MIN_DEGREE) { // 빌려온다
            move_key_left_to_right(left_child, right_child, &(sub_root->key[i - 1]));
            node_delete(right_child, k);
            return;
        }
        else { // 둘다 가난해서 병합 -> 부모키는 없어짐
            if (!left_child->is_leaf) {
                bind_node(sub_root, left_child, right_child, i - 1);
            }
            else {
                bind_leaf_node(sub_root, left_child, right_child, i - 1);
            }
            node_delete(left_child, k);
            return;
        }
        return;
    }
    // 값을 찾았다면 왼쪽 자식으로 내려간다
    if (sub_root->key[i] == k && i <= sub_root->key_count) {
        node *left_child = sub_root->pointer[i];
        node *right_child = sub_root->pointer[i + 1];
        if (left_child->key_count >= MIN_DEGREE) {
            node_delete(left_child, k);
            return;
        }
        if (right_child->key_count >= MIN_DEGREE) {
            move_key_right_to_left(left_child, right_child, &(sub_root->key[i]));
            node_delete(left_child, k);
            return;
        }
        else {
            if (!left_child->is_leaf) {
                bind_node(sub_root, left_child, right_child, i);
            }
            else {
                bind_leaf_node(sub_root, left_child, right_child, i);
            }
            node_delete(left_child, k);
            return;
        }
        return;
    }


    else {
        node *left_child = sub_root->pointer[i];
        node *right_child = sub_root->pointer[i + 1];
        if (left_child->key_count >= MIN_DEGREE) {
            node_delete(left_child, k);
            return;
        }

        if (right_child->key_count >= MIN_DEGREE) {
            move_key_right_to_left(left_child, right_child, &(sub_root->key[i]));
            node_delete(left_child, k);
            return;
        }
        else {
            if (!left_child->is_leaf) {
                bind_node(sub_root, left_child, right_child, i);
            }
            else {
                bind_leaf_node(sub_root, left_child, right_child, i);
            }
            node_delete(left_child, k);
            return;
        }
        return;
    }
}

void move_key_right_to_left(node *left_child, node *right_child, int *parent_key) {
    if (!left_child->is_leaf) {
        *parent_key = right_child->key[1];
        left_child->key[(left_child->key_count) + 1] = right_child->key[1];
        left_child->key_count += 1;
        left_child->pointer[(left_child->key_count) + 1] = right_child->pointer[1];
        left_child->pointer[(left_child->key_count) + 1]->parent = left_child;
        right_child->key_count -= 1;

        for (int j = 1; j <= right_child->key_count; j++) {
            right_child->key[j] = right_child->key[j + 1];
            right_child->pointer[j] = right_child->pointer[j + 1];
        }
        right_child->pointer[(right_child->key_count) + 1] = right_child->pointer[right_child->key_count + 2];
    }
    if (left_child->is_leaf) {
        left_child->key[(left_child->key_count) + 1] = right_child->key[1];
        *parent_key = right_child->key[1];
        left_child->key_count += 1;
        left_child->pointer[(left_child->key_count)] = right_child->pointer[1];
        right_child->key_count -= 1;
        for (int j = 1; j <= right_child->key_count; j++) {
            right_child->key[j] = right_child->key[j + 1];
            right_child->pointer[j] = right_child->pointer[j + 1];
        }

    }
}

void move_key_left_to_right(node *left_child, node *right_child, int *parent_key) {
    if (!right_child->is_leaf) {
        right_child->pointer[(right_child->key_count) + 2] = right_child->pointer[right_child->key_count + 1];
        for (int j = right_child->key_count; j >= 1; j--) {
            right_child->key[j + 1] = right_child->key[j];
            right_child->pointer[j + 1] = right_child->pointer[j];
        }
        *parent_key = left_child->key[left_child->key_count];
        right_child->pointer[1] = left_child->pointer[left_child->key_count + 1];
        right_child->pointer[1]->parent = right_child;
        right_child->key[1] = PRED(right_child->pointer[1]);
        right_child->key_count += 1;
        left_child->key_count -= 1;
    }
    if (right_child->is_leaf) {
        for (int j = right_child->key_count; j >= 1; j--) {
            right_child->key[j + 1] = right_child->key[j];
            right_child->pointer[j + 1] = right_child->pointer[j];
        }
        right_child->key[1] = left_child->key[left_child->key_count];
        right_child->key_count += 1;
        *parent_key = left_child->key[left_child->key_count - 1];
        right_child->pointer[1] = left_child->pointer[left_child->key_count];
        left_child->key_count -= 1;
    }
}

void bind_node(node *parent, node *left_child, node *right_child, int index) {
    left_child->key[left_child->key_count + 1] = parent->key[index];
    for (int j = index; j < parent->key_count; j++) {
        parent->key[j] = parent->key[j + 1];
    }
    for (int j = index + 1; j <= parent->key_count; j++) {
        parent->pointer[j] = parent->pointer[j + 1];
    }
    parent->key_count -= 1;
    for (int j = 1; j <= right_child->key_count; j++) {
        left_child->key[MIN_DEGREE + j] = right_child->key[j];
    }
    for (int j = 1; j <= (right_child->key_count) + 1; j++) {
        left_child->pointer[MIN_DEGREE + j] = right_child->pointer[j];
    }
    left_child->key_count += (right_child->key_count + 1);
    free(right_child);
}

void bind_leaf_node(node *parent, node *left_child, node *right_child, int index) {
    for (int j = index; j < parent->key_count; j++) {
        parent->key[j] = parent->key[j + 1];
    }
    for (int j = index + 1; j <= parent->key_count; j++) {
        parent->pointer[j] = parent->pointer[j + 1];
    }
    parent->key_count -= 1;
    for (int j = 1; j <= right_child->key_count; j++) {
        left_child->key[MIN_DEGREE - 1 + j] = right_child->key[j];
        left_child->pointer[MIN_DEGREE - 1 + j] = right_child->pointer[j];
    }
    left_child->key_count += right_child->key_count;
    free(right_child);
}

int SUCC (node *succ_child) {
    if (succ_child->is_leaf) {
        return succ_child->key[1];
    } else {
        return SUCC(succ_child->pointer[1]);
    }
}

int PRED(node *pred_child){
    if (pred_child->is_leaf){
        return pred_child->key[pred_child->key_count];
    } else {
        return PRED(pred_child->pointer[(pred_child->key_count) + 1]);
    }
}

void display(node *cur_node, int blanks) {
    int i;
    if (cur_node->key_count == 0) {
        printf("tree is empty\n");
        return;
    }
    if (cur_node->is_leaf) {
        for (i = 1; i <= cur_node->key_count; i++) {
            for (int j = 1; j <= blanks; j ++)
                printf("---!");
            printf("%d | ", cur_node->key[i]);
            int *value = (int *)cur_node->pointer[i];
            printf("%d\n", *value);
        }
        return;
    }
    for (i = 1; i <= cur_node->key_count; i++) {
        display(cur_node->pointer[i], blanks + 1);
        for (int j = 1; j <= blanks; j ++)
            printf("---!");
        printf("%d\n", cur_node->key[i]);
    }
    display(cur_node->pointer[i], blanks + 1);
    return;
}
