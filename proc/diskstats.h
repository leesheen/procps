/*
 * libprocps - Library to read proc filesystem
 *
 * Copyright (c) 2003 Fabian Frederick
 * Copyright (C) 2003 Albert Cahalan
 * Copyright (C) 2015 Craig Small <csmall@enc.com.au>
 * Copyright (C) 2016 Jim Warner <james.warner@comcast.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef PROCPS_DISKSTATS_H
#define PROCPS_DISKSTATS_H

#ifdef __cplusplus
extern "C" {
#endif

enum diskstats_item {
    DISKSTATS_noop,                 //        ( never altered )
    DISKSTATS_extra,                //        ( reset to zero )
                                    //  returns        origin, see proc(5)
                                    //  -------        -------------------
    DISKSTATS_NAME,                 //      str        /proc/diskstats
    DISKSTATS_TYPE,                 //    s_int         "
    DISKSTATS_MAJOR,                //    s_int         "
    DISKSTATS_MINOR,                //    s_int         "

    DISKSTATS_READS,                //   ul_int         "
    DISKSTATS_READS_MERGED,         //   ul_int         "
    DISKSTATS_READ_SECTORS,         //   ul_int         "
    DISKSTATS_READ_TIME,            //   ul_int         "
    DISKSTATS_WRITES,               //   ul_int         "
    DISKSTATS_WRITES_MERGED,        //   ul_int         "
    DISKSTATS_WRITE_SECTORS,        //   ul_int         "
    DISKSTATS_WRITE_TIME,           //   ul_int         "
    DISKSTATS_IO_TIME,              //   ul_int         "
    DISKSTATS_WEIGHTED_TIME,        //   ul_int         "

    DISKSTATS_IO_INPROGRESS,        //    s_int         "

    DISKSTATS_DELTA_READS,          //    s_int        derived from above
    DISKSTATS_DELTA_READS_MERGED,   //    s_int         "
    DISKSTATS_DELTA_READ_SECTORS,   //    s_int         "
    DISKSTATS_DELTA_READ_TIME,      //    s_int         "
    DISKSTATS_DELTA_WRITES,         //    s_int         "
    DISKSTATS_DELTA_WRITES_MERGED,  //    s_int         "
    DISKSTATS_DELTA_WRITE_SECTORS,  //    s_int         "
    DISKSTATS_DELTA_WRITE_TIME,     //    s_int         "
    DISKSTATS_DELTA_IO_TIME,        //    s_int         "
    DISKSTATS_DELTA_WEIGHTED_TIME   //    s_int         "
};

enum diskstats_sort_order {
    DISKSTATS_SORT_ASCEND   = +1,
    DISKSTATS_SORT_DESCEND  = -1
};


struct diskstats_result {
    enum diskstats_item item;
    union {
        signed int     s_int;
        unsigned long  ul_int;
        char          *str;
    } result;
};

struct diskstats_stack {
    struct diskstats_result *head;
};

struct diskstats_reap {
    int total;
    struct diskstats_stack **stacks;
};


#define DISKSTATS_TYPE_DISK       -11111
#define DISKSTATS_TYPE_PARTITION  -22222

#define DISKSTATS_GET( info, name, actual_enum, type ) ( { \
    struct diskstats_result *r = procps_diskstats_get( info, name, actual_enum ); \
    r ? r->result . type : 0; } )

#define DISKSTATS_VAL( relative_enum, type, stack, info ) \
    stack -> head [ relative_enum ] . result . type


struct diskstats_info;

int procps_diskstats_new   (struct diskstats_info **info);
int procps_diskstats_ref   (struct diskstats_info  *info);
int procps_diskstats_unref (struct diskstats_info **info);

struct diskstats_result *procps_diskstats_get (
    struct diskstats_info *info,
    const char *name,
    enum diskstats_item item);

struct diskstats_reap *procps_diskstats_reap (
    struct diskstats_info *info,
    enum diskstats_item *items,
    int numitems);

struct diskstats_stack *procps_diskstats_select (
    struct diskstats_info *info,
    const char *name,
    enum diskstats_item *items,
    int numitems);

struct diskstats_stack **procps_diskstats_sort (
    struct diskstats_info *info,
    struct diskstats_stack *stacks[],
    int numstacked,
    enum diskstats_item sortitem,
    enum diskstats_sort_order order);


#ifdef XTRA_PROCPS_DEBUG
# include <proc/xtra-procps-debug.h>
#endif
#ifdef __cplusplus
}
#endif
#endif
