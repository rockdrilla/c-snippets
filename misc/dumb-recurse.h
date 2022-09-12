/* dumb-recurse: VERY dumb preprocessor recurse macros
 *
 * SPDX-License-Identifier: Apache-2.0
 * (c) 2022, Konstantin Demin
 */

#ifndef HEADER_INCLUDED_DUMB_RECURSE
#define HEADER_INCLUDED_DUMB_RECURSE 1

#define DUMB_RECURSE_0(macro,x,...)
#define DUMB_RECURSE_1(macro,x,...) macro(x,__VA_ARGS__)
#define DUMB_RECURSE_2(macro,x,...) DUMB_RECURSE_1(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_3(macro,x,...) DUMB_RECURSE_2(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_4(macro,x,...) DUMB_RECURSE_3(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_5(macro,x,...) DUMB_RECURSE_4(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_6(macro,x,...) DUMB_RECURSE_5(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_7(macro,x,...) DUMB_RECURSE_6(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_8(macro,x,...) DUMB_RECURSE_7(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_9(macro,x,...) DUMB_RECURSE_8(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_10(macro,x,...) DUMB_RECURSE_9(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_11(macro,x,...) DUMB_RECURSE_10(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_12(macro,x,...) DUMB_RECURSE_11(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_13(macro,x,...) DUMB_RECURSE_12(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_14(macro,x,...) DUMB_RECURSE_13(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_15(macro,x,...) DUMB_RECURSE_14(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_16(macro,x,...) DUMB_RECURSE_15(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_17(macro,x,...) DUMB_RECURSE_16(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_18(macro,x,...) DUMB_RECURSE_17(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_19(macro,x,...) DUMB_RECURSE_18(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_20(macro,x,...) DUMB_RECURSE_19(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_21(macro,x,...) DUMB_RECURSE_20(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_22(macro,x,...) DUMB_RECURSE_21(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_23(macro,x,...) DUMB_RECURSE_22(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_24(macro,x,...) DUMB_RECURSE_23(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_25(macro,x,...) DUMB_RECURSE_24(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_26(macro,x,...) DUMB_RECURSE_25(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_27(macro,x,...) DUMB_RECURSE_26(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_28(macro,x,...) DUMB_RECURSE_27(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_29(macro,x,...) DUMB_RECURSE_28(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_30(macro,x,...) DUMB_RECURSE_29(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_31(macro,x,...) DUMB_RECURSE_30(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_32(macro,x,...) DUMB_RECURSE_31(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_33(macro,x,...) DUMB_RECURSE_32(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_34(macro,x,...) DUMB_RECURSE_33(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_35(macro,x,...) DUMB_RECURSE_34(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_36(macro,x,...) DUMB_RECURSE_35(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_37(macro,x,...) DUMB_RECURSE_36(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_38(macro,x,...) DUMB_RECURSE_37(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_39(macro,x,...) DUMB_RECURSE_38(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_40(macro,x,...) DUMB_RECURSE_39(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_41(macro,x,...) DUMB_RECURSE_40(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_42(macro,x,...) DUMB_RECURSE_41(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_43(macro,x,...) DUMB_RECURSE_42(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_44(macro,x,...) DUMB_RECURSE_43(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_45(macro,x,...) DUMB_RECURSE_44(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_46(macro,x,...) DUMB_RECURSE_45(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_47(macro,x,...) DUMB_RECURSE_46(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_48(macro,x,...) DUMB_RECURSE_47(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_49(macro,x,...) DUMB_RECURSE_48(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_50(macro,x,...) DUMB_RECURSE_49(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_51(macro,x,...) DUMB_RECURSE_50(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_52(macro,x,...) DUMB_RECURSE_51(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_53(macro,x,...) DUMB_RECURSE_52(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_54(macro,x,...) DUMB_RECURSE_53(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_55(macro,x,...) DUMB_RECURSE_54(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_56(macro,x,...) DUMB_RECURSE_55(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_57(macro,x,...) DUMB_RECURSE_56(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_58(macro,x,...) DUMB_RECURSE_57(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_59(macro,x,...) DUMB_RECURSE_58(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_60(macro,x,...) DUMB_RECURSE_59(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_61(macro,x,...) DUMB_RECURSE_60(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_62(macro,x,...) DUMB_RECURSE_61(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_63(macro,x,...) DUMB_RECURSE_62(macro,macro(x,__VA_ARGS__),__VA_ARGS__)
#define DUMB_RECURSE_64(macro,x,...) DUMB_RECURSE_63(macro,macro(x,__VA_ARGS__),__VA_ARGS__)

#endif
