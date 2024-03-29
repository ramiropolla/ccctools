#include <mach/mach.h>
#include <mach/mach_error.h>
#include <mach-o/dyld.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
//#include <sys/attr.h>
#include <errno.h>

#include "config.h"

int _NSGetExecutablePath(char *buf, unsigned long *bufsize)
{
    char *path = "/proc/self/exe";
    int len = strlen(path) + 1;
    if (*bufsize < len) {
        *bufsize = len;
        return -1;
    }
    strcpy(buf, path);
    return 0;
}


char *mach_error_string(mach_error_t error_value)
{
  return "Unknown mach error";
}

mach_port_t mach_host_self(void)
{
  return 0;
}

kern_return_t host_info(host_t host, host_flavor_t flavor,
                        host_info_t host_info_out,
                        mach_msg_type_number_t *host_info_outCnt)
{
    if (flavor == HOST_BASIC_INFO) {
        host_basic_info_t      basic_info;

        basic_info = (host_basic_info_t) host_info_out;
        memset(basic_info, 0x00, sizeof(*basic_info));
        basic_info->cpu_type    = CPU_TYPE_EMULATED;
        basic_info->cpu_subtype = CPU_SUBTYPE_EMULATED;
    }
    return KERN_SUCCESS;
}

mach_port_t     mach_task_self_ = 0;

kern_return_t mach_port_deallocate
(
 ipc_space_t task,
 mach_port_name_t name
 )
{
  return 0;
}

kern_return_t vm_allocate
(
 vm_map_t target_task,
 vm_address_t *address,
 vm_size_t size,
        int flags
 )
{

  vm_address_t addr = 0;

  addr = (vm_address_t)calloc(size, sizeof(char));
  if(addr == 0)
    return 1;

  *address = addr;

  return 0;
}

kern_return_t vm_deallocate
(
 vm_map_t target_task,
 vm_address_t address,
        vm_size_t size
 )
{
  //  free((void *)address); leak it here

  return 0;
}

kern_return_t map_fd(int fd, vm_offset_t offset, vm_offset_t *va,
                     boolean_t findspace, vm_size_t size)
{
    void *addr = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, offset);

    if (addr == (void *) -1)
        return 1;

    *va = (vm_offset_t) addr;

    return KERN_SUCCESS;
}

void strmode(mode_t mode, char *bp)
{
  sprintf(bp, "xxxxxxxxxx");
}

#ifndef HAVE_QSORT_R
void *_qsort_thunk = NULL;
int (*_qsort_saved_func)(void *, const void *, const void *) = NULL;

static int _qsort_comparator(const void *a, const void *b);

static int _qsort_comparator(const void *a, const void *b)
{
  return _qsort_saved_func(_qsort_thunk, a, b);
}

void
qsort_r(void *base, size_t nmemb, size_t size, void *thunk,
	int (*compar)(void *, const void *, const void *))
{
  _qsort_thunk = thunk;
  _qsort_saved_func = compar;

  qsort(base, nmemb, size, _qsort_comparator);
}

#endif

vm_size_t       vm_page_size = 4096; // hardcoded to match expectations of darwin
