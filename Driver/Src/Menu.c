/**
 * @file Menu.c
 * @author 九千九百九十九
 * @brief 此为链表操作的底层
 * @version 0.2
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "Menu.h"

/**
 * @brief 注册一个界面
 * 
 * @param pmenu 对应界面的指针
 * @param id 界面ID 0xAB    A：主级数   B：次级数
 * @param pfunc 该界面显示的相关函数
 * @param parent 父级界面
 * @param child 子级界面
 * @param next 同级下一个界面
 * @param prev 同级上一个界面
 */
void Menu_Add(menu_node_t *pmenu,
              uint8_t id,
              menu_update_func_t pfunc,
              menu_node_t *parent,
              menu_node_t *child,
              menu_node_t *next,
              menu_node_t *prev)
{
    pmenu->menu_id = id;
    pmenu->menu_func = pfunc;
    pmenu->menu_parent = parent;
    pmenu->menu_child = child;
    pmenu->menu_next = next;
    pmenu->menu_prev = prev;
}

/**
 * @brief 切换到下一个同级菜单
 * 
 * @details 将当前菜单指针移动到下一个同级菜单，并更新显示
 */
void Menu_MoveNext(void)
{
    if (current_menu && current_menu->menu_next)
    {
        current_menu = current_menu->menu_next; // 切换到下一个菜单
    }
}

/**
 * @brief 切换到上一个同级菜单
 * 
 * @details 将当前菜单指针移动到上一个同级菜单，并更新显示
 */
void Menu_MovePrev(void)
{
    if (current_menu && current_menu->menu_prev)
    {
        current_menu = current_menu->menu_prev; // 切换到上一个菜单
    }
}

/**
 * @brief 进入子菜单
 * 
 * @details 如果当前菜单有子菜单，则进入子菜单并更新显示
 */
void Menu_MoveChild(void)
{
    while (current_menu->menu_prev) //只有在最高级的界面才可以进入下一个级别的界面
    {
        Menu_MovePrev();
    }
    if (current_menu && current_menu->menu_child)
    {
        current_menu = current_menu->menu_child; // 进入子菜单
    }
}

/**
 * @brief 返回父菜单
 * 
 * @details 如果当前菜单有父菜单，则返回父菜单并更新显示
 */
void Menu_MoveParent(void)
{
    while (current_menu->menu_prev)
    {
        Menu_MovePrev();
    }
    if (current_menu && current_menu->menu_parent)
    {
        current_menu = current_menu->menu_parent; // 返回父菜单
    }
}

/**
 * @brief 更新当前菜单显示
 * 
 * @details 调用当前菜单的更新函数，刷新显示缓冲区内容
 *          此函数应定期被任务调度器调用，以保持显示内容的实时性
 */
void Menu_Update(void)
{
    if (current_menu && current_menu->menu_func)
    {
        current_menu->menu_func(); // 调用当前菜单的更新函数
    }
}
