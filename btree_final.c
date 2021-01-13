//#include <stdio.h>
//#include <stdbool.h>
//#include <stdlib.h>
//
//#define MIN_DEGREE 3
//#define MAX_KEY (MIN_DEGREE*2 - 1)
//#define MIN_KEY (MIN_DEGREE - 1)
//
//typedef struct node {
//    bool is_leaf;
//    int key[MAX_KEY + 1], key_count;
//    struct node *linker[MAX_KEY + 2];
//}node;
//
//void b_Tree_create(node **root);
//void b_tree_search(node *sub_root, int k);
//void b_tree_insert(node **root, int k);
//void b_tree_delete(node *sub_root, node **root, int k);
//void split_node(node *parent_node, int child_index);
//void insert_node(node *sub_root, int k);
//void delete_node(node *sub_root, int k);
//void bind_node(node *left_child, node *right_child, node *parent_node, int index);
//int PRED(node *pred_child);
//int SUCC(node *succ_child);
//void display(node *cur_node, int blanks);
//
//int main() {
//    node *root;
//    b_Tree_create(&root);
//    b_tree_insert(&root, 10);
//    b_tree_insert(&root, 20);
//    b_tree_insert(&root, 30);
//    b_tree_insert(&root, 40);
//    b_tree_insert(&root, 50);
//    b_tree_insert(&root, 60);
//    b_tree_insert(&root, 70);
//    b_tree_insert(&root, 80);
//    b_tree_insert(&root, 90);
//    b_tree_insert(&root, 100);
//    b_tree_insert(&root, 110);
//    b_tree_insert(&root, 120);
//    b_tree_insert(&root, 130);
//    b_tree_insert(&root, 140);
//    b_tree_insert(&root, 150);
//    b_tree_insert(&root, 160);
//    b_tree_insert(&root, 170);
//    b_tree_insert(&root, 180);
//    b_tree_insert(&root, 190);
//    b_tree_insert(&root, 200);
//    b_tree_insert(&root, 210);
//    b_tree_insert(&root, 220);
//    b_tree_insert(&root, 230);
//    b_tree_insert(&root, 240);
//    b_tree_insert(&root, 250);
//    b_tree_insert(&root, 260);
//    b_tree_insert(&root, 9);
//    b_tree_insert(&root, 39);
//    b_tree_insert(&root, 101);
//    b_tree_insert(&root, 102);
//    b_tree_insert(&root, 103);
//    b_tree_insert(&root, 104);
//    b_tree_insert(&root, 161);
//    b_tree_insert(&root, 191);
//    b_tree_insert(&root, 251);
//
//    b_tree_delete(root, &root, 103);
//    b_tree_delete(root, &root, 70);
//    b_tree_delete(root, &root, 130);
//    b_tree_delete(root, &root, 104);
//    b_tree_delete(root, &root, 60);
//    b_tree_delete(root, &root, 120);
//    b_tree_delete(root, &root, 160);
//    b_tree_delete(root, &root, 180);
//    b_tree_delete(root, &root, 250);
//    b_tree_delete(root, &root, 20);
//    b_tree_delete(root, &root, 80);
//    b_tree_delete(root, &root, 102);
//    b_tree_delete(root, &root, 50);
//    b_tree_delete(root, &root, 90);
//    b_tree_delete(root, &root, 200);
//    display(root, 0);
//}
//
//void b_Tree_create(node **root){
//    node *new_node = (node *)malloc(sizeof(node));
//    new_node->is_leaf = true;
//    new_node->key_count = 0;
//    *root = new_node;
//}
//
//void b_tree_search(node *sub_root, int k){
//    int i = 1;
//    while(i < sub_root->key_count && k > sub_root->key[i]){
//        i = i + 1;
//    }
//    if (i <= sub_root->key_count && k == sub_root->key[i]){
//        printf("success");
//    } else if(sub_root->is_leaf == true){
//        printf("fail");
//    } else {
//        b_tree_search(sub_root->linker[i], i);
//    }
//}
//
//void split_node(node *parent_node, int child_index){
//    node *right_child_node = (node *)malloc(sizeof(node));
//    node *left_child_node = parent_node->linker[child_index];
//    right_child_node->is_leaf = left_child_node -> is_leaf;
//    right_child_node->key_count = MIN_KEY;
//    for (int i = 1; i <= MIN_KEY; i ++) {
//        right_child_node->key[i] = left_child_node->key[i + MIN_DEGREE];
//    }
//    if (!left_child_node->is_leaf) {
//        for (int i = 1; i <= MIN_DEGREE; i++){
//            right_child_node->linker[i] = left_child_node->linker[i + MIN_DEGREE];
//        }
//    }
//    right_child_node->linker[0] = parent_node;
//    left_child_node->key_count = MIN_KEY;
//    for (int i = parent_node->key_count + 1; i >= child_index + 1; i--){
//        parent_node->linker[i + 1] = parent_node->linker[i];
//    }
//    parent_node->linker[child_index + 1] = right_child_node;
//
//    for (int i = parent_node->key_count; i >= child_index; i--){
//        parent_node->key[i + 1] = parent_node->key[i];
//    }
//    parent_node->key[child_index] = left_child_node->key[MIN_DEGREE];
//    parent_node->key_count += 1;
//}
//
//void b_tree_insert(node **root, int k){
//    node *curr_root = *root;
//    if((*root)->key_count == MAX_KEY) {
//        node *new_root = (node*)malloc(sizeof(node));
//        *root = new_root;
//        new_root->is_leaf = false;
//        new_root->key_count = 0;
//        new_root->linker[1] = curr_root;
//        //child[0]�� �θ� �߰�
//        curr_root->linker[0] = new_root;
//        split_node(new_root, 1);
//        insert_node(new_root, k);
//    }
//    else {
//        insert_node(curr_root, k);
//    }
//}
//
//void insert_node(node *sub_root, int k) {
//    int i = sub_root->key_count;
//    if (sub_root->is_leaf){
//        while (i >= 1 && k < sub_root->key[i])
//        {
//            sub_root->key[i + 1] = sub_root->key[i];
//            i -= 1;
//        }
//        sub_root->key[i + 1] = k;
//        sub_root->key_count += 1;
//    }
//    else {
//        while (i >= 1 && k < sub_root->key[i])
//        {
//            i -= 1;
//        }
//        i += 1;
//        if (sub_root->linker[i]->key_count == MAX_KEY) {
//            split_node(sub_root, i);
//            if (k > sub_root->key[i]) {
//                i += 1;
//            }
//        }
//        insert_node(sub_root->linker[i], k);
//    }
//}
//
//void b_tree_delete(node *sub_root, node **root, int k){
//    if ((*root)->key_count == 0){
//        if ((*root)->is_leaf){
//            printf("tree is empty\n");
//            return;
//        }
//    }
//    delete_node(sub_root, k);
//    if ((*root)->key_count == 0){
//        if ((*root)->is_leaf){
//            printf("tree is empty\n");
//            free(*root);
//            b_Tree_create(root);
//            return;
//        } else {
//            node *old_root = *root;
//            *root = (*root)->linker[1];
//            free(old_root);
//            return;
//        }
//    }
//}
//
//void delete_node(node *sub_root, int k){
//    if (sub_root->is_leaf){
//        int original_key_count = sub_root->key_count;
//        for (int i = 1; i <= sub_root->key_count; i ++){
//            if (sub_root->key[i] == k){
//                for (int j = i; j < sub_root->key_count; j ++){
//                    sub_root->key[j] = sub_root->key[j + 1];
//                }
//                sub_root->key_count -= 1;
//            }
//        }
//        if (original_key_count == sub_root->key_count){
//            printf("%d not in b-tree\n", k);
//        }
//        return;
//    }
//
//    int i = 1;
//    while(sub_root->key[i] < k && i <= sub_root->key_count){
//        i += 1;
//    }
//    // 값을 찾은 경우
//    if (sub_root->key[i] == k && sub_root->key_count >= i){
//        if (sub_root->linker[i]->key_count >= MIN_DEGREE){
//            int pred = PRED(sub_root->linker[i]);
//            sub_root->key[i] = pred;
//            delete_node(sub_root->linker[i], pred);
//        }
//        else if (sub_root->linker[i + 1]->key_count >= MIN_DEGREE){
//            int succ = SUCC(sub_root->linker[i + 1]);
//            sub_root->key[i] = succ;
//            delete_node(sub_root->linker[i + 1], succ);
//        } else {
//            bind_node(sub_root->linker[i], sub_root->linker[i + 1], sub_root, i);
//            delete_node(sub_root->linker[i], k);
//        }
//    }
//        // 오른쪽 맨끝까지 간 경우 -> 병합 시, 왼쪽꺼랑 붙여준다
//    else {
//        if (i == sub_root->key_count + 1){
//            if (sub_root->linker[i]->key_count >= MIN_DEGREE){
//                delete_node(sub_root->linker[i], k);
//            } else {
//                if (sub_root->linker[i - 1]->key_count >= MIN_DEGREE){
//                    for (int j = sub_root->linker[i]->key_count; j >= 1; j--){
//                        sub_root->linker[i]->key[j + 1] = sub_root->linker[i]->key[j];
//                    }
//
//                    int left_sibling_key_count = sub_root->linker[i - 1]->key_count;
//                    if (!sub_root->linker[i]->is_leaf){
//                        for (int j = (sub_root->linker[i]->key_count) + 1; j >= 1; j--){
//                            sub_root->linker[i]->linker[j + 1] = sub_root->linker[i]->linker[j];
//                        }
//                        sub_root->linker[i]->linker[1] = sub_root->linker[i - 1]->linker[left_sibling_key_count + 1];
//                    }
//                    sub_root->linker[i]->key[1] = sub_root->key[i - 1];
//                    sub_root->linker[i]->key_count += 1;
//                    sub_root->key[i - 1] = sub_root->linker[i - 1]->key[left_sibling_key_count];
//                    sub_root->linker[i - 1]->key_count -= 1;
//                    delete_node(sub_root->linker[i], k);
//                } else {
//                    bind_node(sub_root->linker[i - 1], sub_root->linker[i], sub_root, i - 1);
//                    delete_node(sub_root->linker[i - 1], k);
//                }
//            }
//        }
//            // 오른쪽 끝까지 안 간 경우 -> 병합 시, 오른쪽꺼랑 붙여준다
//        else {
//            if (sub_root->linker[i]->key_count >= MIN_DEGREE){
//                delete_node(sub_root->linker[i], k);
//            } else {
//                if (sub_root->linker[i + 1]->key_count >= MIN_DEGREE){
//                    sub_root->linker[i]->key[MIN_DEGREE] = sub_root->key[i];
//                    sub_root->linker[i]->key_count += 1;
//                    sub_root->key[i] = sub_root->linker[i + 1]->key[1];
//                    sub_root->linker[i + 1]->key_count -= 1;
//                    sub_root->linker[i]->linker[MIN_DEGREE + 1] = sub_root->linker[i + 1]->linker[1];
//                    for (int j = 1; j <= sub_root->linker[i + 1]->key_count; j ++){
//                        sub_root->linker[i + 1]->key[j] = sub_root->linker[i + 1]->key[j + 1];
//                    }
//                    if (!sub_root->is_leaf){
//                        for (int j = 1; j <= (sub_root->linker[i + 1]->key_count) + 1; j ++){
//                            sub_root->linker[i + 1]->linker[j] = sub_root->linker[i + 1]->linker[j + 1];
//                        }
//                    }
//                    delete_node(sub_root->linker[i], k);
//                } else {
//                    bind_node(sub_root->linker[i], sub_root->linker[i + 1], sub_root, i);
//                    delete_node(sub_root->linker[i], k);
//                }
//            }
//        }
//    }
//}
//
//void bind_node(node *left_child, node *right_child, node *parent_node, int index){
//    left_child->key[left_child->key_count + 1] = parent_node->key[index];
//    for (int j = index; j < parent_node->key_count; j ++){
//        parent_node->key[j] = parent_node->key[j + 1];
//    }
//    for (int j = index + 1; j <= parent_node->key_count; j ++){
//        parent_node->linker[j] = parent_node->linker[j + 1];
//    }
//    parent_node->key_count -= 1;
//
//    for (int j = 1; j <= right_child->key_count; j ++){
//        left_child->key[MIN_DEGREE + j] = right_child->key[j];
//    }
//    if (!left_child->is_leaf){
//        for (int j = 1; j <= (right_child->key_count) + 1; j ++){
//            left_child->linker[MIN_DEGREE + j] = right_child->linker[j];
//        }
//    }
//    left_child->key_count = (left_child->key_count) + (right_child->key_count) + 1;
//    free(right_child);
//}
//
//int PRED(node *pred_child){
//    if (pred_child->is_leaf){
//        return pred_child->key[pred_child->key_count];
//    } else {
//        return PRED(pred_child->linker[(pred_child->key_count) + 1]);
//    }
//}
//
//int SUCC(node *succ_child){
//    if (succ_child->is_leaf){
//        return succ_child->key[1];
//    } else {
//        return SUCC(succ_child->linker[1]);
//    }
//}
//
//void display(node *cur_node, int blanks) {
//    int i;
//    if (cur_node->is_leaf) {
//        for (i = 1; i <= cur_node->key_count; i++) {
//            for (int j = 1; j <= blanks; j ++)
//                printf("---!");
//            printf("%d\n", cur_node->key[i]);
//        }
//        return;
//    }
//    for (i = 1; i <= cur_node->key_count; i++) {
//        display(cur_node->linker[i], blanks + 1);
//        for (int j = 1; j <= blanks; j ++)
//            printf("---!");
//        printf("%d\n", cur_node->key[i]);
//    }
//    display(cur_node->linker[i], blanks + 1);
//    return;
//}