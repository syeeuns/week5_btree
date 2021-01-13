#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MIN_DEGREE 4
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
void node_insert(node *sub_root, int k, int v);
void node_split(node *parent, int child_index);
void node_delete(node *sub_root, int k);
void move_key_right_to_left(node *left_child, node *right_child, int *parent_key);
void move_key_left_to_right(node *left_child, node *right_child, int *parent_key);
void bind_node(node *parent, node *left_child, node *right_child, int index);
void bind_leaf_node(node *parent, node *left_child, node *right_child, int index);
void display(node *cur_node, int blanks);
void test_case(node **root);
int SUCC(node *succ_child);

int main() {
    node *root;
    b_plus_tree_create(&root);
    test_case(&root);
    display(root, 0);
}

void test_case(node **root) {
    int* out_arr = (int*)malloc(sizeof(int) * 1000000);
    for (int i = 0; i < 1000000; i++) {
        out_arr[i] = i;
    }
    for (int i = 0; i < 1000000; i++)
    {
        int j = i + rand() / (RAND_MAX / (1000000 - i) + 1);
        int t = out_arr[j];
        out_arr[j] = out_arr[i];
        out_arr[i] = t;
    }
    for (int i = 0; i < 1000000; i++) {
        int r = out_arr[i];
        b_plus_tree_insert(root, r, r);
    }
    for (int i = 0; i < 1000000; i++) {
        int r = out_arr[i];
        b_plus_tree_delete(*root, root, r);
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

    if (right_child->is_leaf) {
        right_child->key_count = MIN_DEGREE;
        for (int i = 1; i <= MIN_DEGREE; i ++) {
            right_child->key[i] = left_child->key[i + MIN_DEGREE - 1];
        }
        for (int i = 1; i <= MIN_DEGREE; i ++) {
            right_child->pointer[i] = left_child->pointer[i + MIN_DEGREE - 1];
        }
        left_child->right = right_child;
        right_child->left = left_child;
    }
    else {
        right_child->key_count = (MIN_DEGREE - 1);
        for (int i = 1; i <= (MIN_DEGREE - 1); i ++) {
            right_child->key[i] = left_child->key[i + MIN_DEGREE];
        }
        for (int i = 1; i <= MIN_DEGREE; i++) {
            right_child->pointer[i] = left_child->pointer[i + MIN_DEGREE];
        }
    }
    right_child->parent = parent;  //오른쪽 자식에도 부모 추가
    left_child->key_count = (MIN_DEGREE - 1);
    for (int i = parent->key_count + 1; i >= child_index + 1; i--) {
        parent->pointer[i + 1] = parent->pointer[i];
    }
    parent->pointer[child_index + 1] = right_child;  //오른쪽 자식 넣기
    for (int i = parent->key_count; i >= child_index; i--) {
        parent->key[i + 1] = parent->key[i];
    }
    parent->key[child_index] = left_child->key[MIN_DEGREE];  //중앙값 올리기
    parent->key_count += 1;
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
            *root = (*root)->pointer[1];
            free(old_root);
            return;
        }
    }
}

void node_delete(node *sub_root, int k) {
    // 리프노드일 때
    if (sub_root->is_leaf){
        int original_key_count = sub_root->key_count;
        for (int i = 1; i <= sub_root->key_count; i ++) {
            if (sub_root->key[i] == k){
                free(sub_root->pointer[i]);
                for (int j = i; j < sub_root->key_count; j ++) {
                    sub_root->key[j] = sub_root->key[j + 1];
                    sub_root->pointer[j] = sub_root->pointer[j + 1];
                }
                sub_root->key_count -= 1;
                break;
            }
        }
        if (original_key_count == sub_root->key_count) {
            printf("%d not in b-tree\n", k);
        }
        return;
    }
    // 리프노드가 아니라면, key를 순회하면서 삭제하는 값을 찾을 수 있는 위치로 이동
    int i = 1;
    while(sub_root->key[i] < k && i <= sub_root->key_count) {
        i += 1;
    }
    // 삭제할 값을 찾았다면, SUCC로 대체 될 현재 위치는 i, i의 바로 오른쪽 자식을 검사
    if (sub_root->key[i] == k && i <= sub_root->key_count) {
        node *left_child = sub_root->pointer[i];
        node *right_child = sub_root->pointer[i + 1];
        if (right_child->key_count >= MIN_DEGREE) {  // 오른쪽 자식이 MIN_DEGREE 이상의 키를 가지고 있으면
            node_delete(right_child, k);
            sub_root->key[i] = SUCC(sub_root->pointer[i + 1]);  // 현재 키를 SUCC로 대체
            return;
        }
        // 오른쪽 자식노드의 키 개수가 t 미만이어서 왼쪽 자식을 검사
        if (left_child->key_count >= MIN_DEGREE) {  // 왼쪽 자식이 MIN_DEGREE 이상의 키를 가지고 있으면
            move_key_left_to_right(left_child, right_child, &(sub_root->key[i]));
            node_delete(right_child, k);
            return;
        } else {  // 왼쪽, 오른쪽 자식 모두의 키 개수가 t개 미만이어서 부모키를 가져와 병합 수행
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
    // 값을 찾지 못했을 때
    if (i == sub_root->key_count + 1) { // 노드의 키 안에 k보다 큰 키가 존재하지 않아 가장 오른쪽 자식노드 검사
        node *left_child = sub_root->pointer[i - 1];
        node *right_child = sub_root->pointer[i];
        if (right_child->key_count >= MIN_DEGREE) { // 오른쪽 자식노드의 키 개수가 t 이상일 때
            node_delete(right_child, k);
            return;
        }
        // 오른쪽 자식노드의 키 개수가 t 미만이어서 왼쪽 자식을 검사
        if (left_child->key_count >= MIN_DEGREE) { // 왼쪽 자식노드의 키 개수가 t 이상일 때 가장 마지막 키를 가져옴
            move_key_left_to_right(left_child, right_child, &(sub_root->key[i - 1]));
            node_delete(right_child, k);
            return;
        }
        else { // 왼쪽, 오른쪽 자식 모두의 키 개수가 t개 미만이어서 부모키를 가져와 병합 수행
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
    else {  // 가장 오른쪽 노드를 검사하는 경우가 아니라면
        node *left_child = sub_root->pointer[i];
        node *right_child = sub_root->pointer[i + 1];
        if (left_child->key_count >= MIN_DEGREE) { // 왼쪽 자식 노드의 key 개수가 t개 이상
            node_delete(left_child, k);
            return;
        }
        // 왼쪽 자식 노드의 key 개수가 t개 미만이어서 오른쪽 자식을 검사
        if (right_child->key_count >= MIN_DEGREE) { // 오른쪽 자식노드의 키 개수가 t 이상일 때 가장 처음 키를 가져옴
            move_key_right_to_left(left_child, right_child, &(sub_root->key[i]));
            node_delete(left_child, k);
            return;
        }
        else { // 왼쪽, 오른쪽 자식노드의 key 개수가 t개 미만이기 때문에 부모키를 가져와 병합을 수행
            if (!left_child->is_leaf) {
                bind_node(sub_root, left_child, right_child, i);
            }
            else {
                bind_leaf_node(sub_root, left_child, right_child, i);
            }
            node_delete(sub_root->pointer[i], k);
            return;
        }
        return;
    }
}

void move_key_right_to_left(node *left_child, node *right_child, int *parent_key) {
    if (!left_child->is_leaf) {
        left_child->key[(left_child->key_count) + 1] = *parent_key;
        left_child->key_count += 1;
        left_child->pointer[(left_child->key_count) + 1] = right_child->pointer[1];
        left_child->pointer[(left_child->key_count) + 1]->parent = left_child;
        right_child->key_count -= 1;
        *parent_key = right_child->key[1];
        for (int j = 1; j <= right_child->key_count; j++) {
            right_child->key[j] = right_child->key[j + 1];
            right_child->pointer[j] = right_child->pointer[j + 1];
        }
        right_child->pointer[(right_child->key_count) + 1] = right_child->pointer[right_child->key_count + 2];
    }
    if (left_child->is_leaf) {
        left_child->key[(left_child->key_count) + 1] = *parent_key;
        left_child->key_count += 1;
        left_child->pointer[(left_child->key_count)] = right_child->pointer[1];
        right_child->key_count -= 1;
        for (int j = 1; j <= right_child->key_count; j++) {
            right_child->key[j] = right_child->key[j + 1];
            right_child->pointer[j] = right_child->pointer[j + 1];
        }
        *parent_key = right_child->key[1];
    }
}

void move_key_left_to_right(node *left_child, node *right_child, int *parent_key) {
    if (!right_child->is_leaf) {
        right_child->pointer[(right_child->key_count) + 2] = right_child->pointer[right_child->key_count + 1];
        for (int j = right_child->key_count; j >= 1; j--) {
            right_child->key[j + 1] = right_child->key[j];
            right_child->pointer[j + 1] = right_child->pointer[j];
        }
        right_child->key[1] = *parent_key;
        right_child->key_count += 1;
        *parent_key = left_child->key[left_child->key_count];
        right_child->pointer[1] = left_child->pointer[left_child->key_count + 1];
        right_child->pointer[1]->parent = right_child;
        left_child->key_count -= 1;
    }
    if (right_child->is_leaf) {
        for (int j = right_child->key_count; j >= 1; j--) {
            right_child->key[j + 1] = right_child->key[j];
            right_child->pointer[j + 1] = right_child->pointer[j];
        }
        right_child->key[1] = left_child->key[left_child->key_count];
        right_child->key_count += 1;
        *parent_key = left_child->key[left_child->key_count];
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