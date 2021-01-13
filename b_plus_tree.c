#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MIN_DEGREE 2
#define MAX_KEY (MIN_DEGREE*2 - 1)

typedef struct _node {
    bool is_leaf;
    int key[MAX_KEY + 1], key_count;
    struct _node *pointer[MAX_KEY + 2], *parent, *left, *right;
} node;

void b_plus_tree_create(node **root);
void b_plus_tree_insert(node **root, int k, int value);
void b_plus_tree_delete(node *sub_root, node **root, int k);
void node_split(node *parent, int child_index);
void node_insert(node *sub_root, int k, int v);
void node_delete(node *sub_root, int k);
void bind_node(node *parent, node *left_child, node *right_child, int index);
void bind_leaf_node(node *parent, node *left_child, node *right_child, int index);
int SUCC(node *succ_child);
void display(node *cur_node, int blanks);

int main() {
    node *root;
    b_plus_tree_create(&root);

    b_plus_tree_insert(&root, 10, 1);
    b_plus_tree_insert(&root, 20, 2);
    b_plus_tree_insert(&root, 30, 3);
    b_plus_tree_insert(&root, 40, 4);
    b_plus_tree_insert(&root, 9, 9);
    b_plus_tree_insert(&root, 50, 5);
    b_plus_tree_insert(&root, 60, 6);
    b_plus_tree_insert(&root, 70, 7);
    b_plus_tree_insert(&root, 80, 8);
    b_plus_tree_insert(&root, 90, 90000);
    b_plus_tree_insert(&root, 100, 10);
    b_plus_tree_insert(&root, 110, 11);
    b_plus_tree_insert(&root, 120, 12);
    b_plus_tree_insert(&root, 130, 13);
    b_plus_tree_insert(&root, 140, 14);
    b_plus_tree_insert(&root, 150, 15);
    b_plus_tree_insert(&root, 160, 16);
    b_plus_tree_insert(&root, 170, 17);
    b_plus_tree_insert(&root, 180, 18);
    b_plus_tree_insert(&root, 190, 19);
    b_plus_tree_insert(&root, 200, 20);
    b_plus_tree_insert(&root, 210, 21);
    b_plus_tree_insert(&root, 220, 22);
    b_plus_tree_insert(&root, 230, 23);
    b_plus_tree_insert(&root, 240, 24);
    b_plus_tree_insert(&root, 250, 25);
    b_plus_tree_insert(&root, 260, 26);
    b_plus_tree_insert(&root, 39, 39);
    b_plus_tree_insert(&root, 101, 101);
    b_plus_tree_insert(&root, 102, 102);
    b_plus_tree_insert(&root, 103, 103);
    b_plus_tree_insert(&root, 104, 104);
    b_plus_tree_insert(&root, 161, 161);
    b_plus_tree_insert(&root, 191, 191);
    b_plus_tree_insert(&root, 251, 251);

    b_plus_tree_delete(root, &root, 20);
    b_plus_tree_delete(root, &root, 103);
    b_plus_tree_delete(root, &root, 70);
    b_plus_tree_delete(root, &root, 30);
    b_plus_tree_delete(root, &root, 130);
    b_plus_tree_delete(root, &root, 104);
    b_plus_tree_delete(root, &root, 60);
    b_plus_tree_delete(root, &root, 120);
    b_plus_tree_delete(root, &root, 160);
    b_plus_tree_delete(root, &root, 180);
    b_plus_tree_delete(root, &root, 250);
    b_plus_tree_delete(root, &root, 20);
    b_plus_tree_delete(root, &root, 80);
    b_plus_tree_delete(root, &root, 102);
    b_plus_tree_delete(root, &root, 50);
    b_plus_tree_delete(root, &root, 90);

    display(root, 0);
}

void b_plus_tree_create(node **root) {
    node *new_node = (node *)malloc(sizeof(node));
    new_node->is_leaf = true;
    new_node->key_count = 0;
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;
    *root = new_node;
}

void node_split(node *parent, int child_index) {
    node *right_child = (node *)malloc(sizeof(node));
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

void b_plus_tree_insert(node **root, int k, int v) {
    node *curr_root = *root;
    if((*root)->key_count == MAX_KEY) {
        node *new_root = (node*)malloc(sizeof(node));
        *root = new_root;
        new_root->is_leaf = false;
        new_root->key_count = 0;
        new_root->pointer[1] = curr_root;
        new_root->parent = NULL;
        new_root->left = NULL;
        new_root->right = NULL;
        curr_root->parent = new_root;
        node_split(new_root, 1);
        node_insert(new_root, k, v);
    }
    else {
        node_insert(curr_root, k, v);
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
                if (i == 1) {
                    for (int j = 1; j <= sub_root->parent->key_count; j++) {
                        if (sub_root->parent->key[j] == k) {
                            sub_root->parent->key[j] = sub_root->key[2];
                            break;
                        }
                    }
                }
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
        else if (left_child->key_count >= MIN_DEGREE) {  // 왼쪽 자식이 MIN_DEGREE 이상의 키를 가지고 있으면
            if (!right_child->is_leaf) {
                right_child->pointer[(right_child->key_count) + 2] = right_child->pointer[right_child->key_count + 1];
            }
            for (int j = right_child->key_count; j >= 1; j--) {
                right_child->key[j + 1] = right_child->key[j];
                right_child->pointer[j + 1] = right_child->pointer[j];
            }
            if (right_child->is_leaf) {
                right_child->key[1] = left_child->key[left_child->key_count];
            } else {
                right_child->key[1] = sub_root->key[i];
            }
            right_child->key_count += 1;
            sub_root->key[i] = left_child->key[left_child->key_count];
            if (right_child->is_leaf) {
                right_child->pointer[1] = left_child->pointer[left_child->key_count];
            }
            else {
                right_child->pointer[1] = left_child->pointer[left_child->key_count + 1];
                right_child->pointer[1]->parent = right_child;
            }
            left_child->key_count -= 1;
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
            if (!right_child->is_leaf) {
                right_child->pointer[(right_child->key_count) + 2] = right_child->pointer[right_child->key_count + 1];
            }
            for (int j = right_child->key_count; j >= 1; j--) {
                right_child->key[j + 1] = right_child->key[j];
                right_child->pointer[j + 1] = right_child->pointer[j];
            }
            right_child->key[1] = sub_root->key[i];
            right_child->key_count += 1;
            sub_root->key[i - 1] = left_child->key[left_child->key_count];
            right_child->pointer[1] = left_child->pointer[left_child->key_count + 1];
            if (!right_child->is_leaf) {
                right_child->pointer[1]->parent = right_child;
            }
            left_child->key_count -= 1;
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
            left_child->key[(left_child->key_count) + 1] = sub_root->key[i];
            left_child->key_count += 1;
            sub_root->key[i] = right_child->key[1];
            if (!left_child->is_leaf) {
                left_child->pointer[(left_child->key_count) + 1] = right_child->pointer[1];
                left_child->pointer[(left_child->key_count) + 1]->parent = left_child;
            } else {
                left_child->pointer[(left_child->key_count)] = right_child->pointer[1];
            }
            right_child->key_count -= 1;
            for (int j = 1; j <= right_child->key_count; j++) {
                right_child->key[j] = right_child->key[j + 1];
                right_child->pointer[j] = right_child->pointer[j + 1];
            }
            if (!right_child->is_leaf) {
                right_child->pointer[(right_child->key_count) + 1] = right_child->pointer[right_child->key_count + 2];
            }
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

int PRED (node *pred_child) {
    if (pred_child->is_leaf) {
        return pred_child->key[pred_child->key_count];
    } else {
        return PRED(pred_child->pointer[(pred_child->key_count) + 1]);
    }
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
