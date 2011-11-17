#ifndef _HGE_POSTOFFICE_H_
#define _HGE_POSTOFFICE_H_

#include "HGE_types.h"
#include "HGE_queue.h"
#include "HGE_message.h"

void HGE_postoffice_init_system(void);
void HGE_postoffice_clean_system(void);

int HGE_postoffice_default_filter(HGE_queue_t* inbox, HGE_message_t* msg);
int HGE_postoffice_default_handler(HGE_message_t* msg);

void HGE_postoffice_set_fh(void* ptr, HGE_typeinfo_t* type);
void HGE_postoffice_set_filter(void* ptr, HGE_message_filter_t filter);
void HGE_postoffice_set_handler(void* ptr, HGE_message_handler_t handler);
void HGE_postoffice_set_default_filter(HGE_message_filter_t filter);
void HGE_postoffice_set_default_handler(HGE_message_handler_t handler);
HGE_message_filter_t  HGE_postoffice_get_filter(void* ptr);
HGE_message_handler_t HGE_postoffice_get_handler(void* ptr);
HGE_message_filter_t  HGE_postoffice_get_default_filter(void);
HGE_message_handler_t HGE_postoffice_get_default_handler(void);

void HGE_postoffice_send(Uint32 when, void* sender, void* receiver, Uint32 id, HGE_argv_t* argv);
void HGE_postoffice_send_msg(Uint32 when, HGE_message_t* msg);
void HGE_postoffice_process(void);

void         HGE_postoffice_purge(void* ptr);
HGE_queue_t* HGE_postoffice_steal(void* ptr);
HGE_queue_t* HGE_postoffice_copy(void* ptr);
Uint32       HGE_postoffice_resume(void* ptr);
Uint32       HGE_postoffice_suspend(void* ptr);

#endif /* _HGE_POSTOFFICE_H_ */
