#ifndef __Menu_h
#define __Menu_h

#include "main.h"

// 菜单刷新函数指针类型
typedef void (*menu_update_func_t)(void);

// 菜单节点结构
typedef struct menu_node_t
{
    menu_update_func_t menu_func; // 更新显示的函数
    uint8_t menu_id; //界面标识
    struct menu_node_t *menu_parent; // 父菜单指针
    struct menu_node_t *menu_child; // 子菜单指针
    struct menu_node_t *menu_next; // 同级下一个菜单
    struct menu_node_t *menu_prev; // 同级上一个菜单
} menu_node_t;

// 全局菜单当前指针
extern menu_node_t *current_menu;

// 菜单导航函数
void Menu_Add(menu_node_t *pmenu,
              uint8_t id,
              menu_update_func_t pfunc,
              menu_node_t *menu_parent,
              menu_node_t *menu_child,
              menu_node_t *menu_next,
              menu_node_t *menu_prev); //注册一个页面
void Menu_MoveNext(void); // 切换到下一个同级菜单
void Menu_MovePrev(void); // 切换到上一个同级菜单
void Menu_MoveChild(void); // 进入子菜单
void Menu_MoveParent(void); // 返回父菜单
void Menu_Update(void); // 更新当前菜单显示

// 菜单初始化
void Menu_Init(void);

#endif
