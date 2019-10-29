#pragma once

#include <ftl/utils.hpp>
#include <ftl/whereami/whereami.hpp>
#include <vector>
#include <string>
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"

#ifdef ONLY_LD
#define LD_CAT FtlLdToolCategory
#else
#define LD_CAT FtlCompilerToolCategory
#endif

#ifndef ONLY_LD
static llvm::cl::OptionCategory FtlCompilerToolCategory("compiler options");
#else
static llvm::cl::OptionCategory FtlLdToolCategory("ld options");
#endif

/// begin ld options
static cl::list <std::string> L_opt(
        "L",
        cl::desc("Add directory to library search path"),
        cl::cat(LD_CAT),
        cl::Prefix,
        cl::ZeroOrMore);
static cl::list <std::string> l_opt(
        "l",
        cl::desc("Root name of library to link"),
        cl::cat(LD_CAT),
        cl::Prefix,
        cl::ZeroOrMore);
static cl::opt <std::string> o_opt(
        "o",
        cl::desc("Write output to <file>"),
        cl::cat(LD_CAT));
static cl::list <std::string> input_filename_opt(
        cl::Positional,
        cl::desc("<input file> ..."),
        cl::cat(LD_CAT),
        cl::OneOrMore);
/// End of ld options

#ifndef ONLY_LD
/// begin c/c++ options/
static cl::opt<bool> c_opt(
        "c",
        cl::desc("Only run preprocess, compile, and assemble steps"),
        cl::cat(FtlCompilerToolCategory));
static cl::list <std::string> D_opt(
        "D",
        cl::desc("Define <macro> to <value> (or 1 if <value> omitted)"),
        cl::ZeroOrMore,
        cl::Prefix,
        cl::cat(FtlCompilerToolCategory));
static cl::list <std::string> include_opt(
        "include",
        cl::desc("Include file before parsing"),
        cl::cat(FtlCompilerToolCategory),
        cl::ZeroOrMore);
static cl::list <std::string> I_opt(
        "I",
        cl::desc("Add directory to include search path"),
        cl::cat(FtlCompilerToolCategory),
        cl::Prefix,
        cl::ZeroOrMore);
static cl::opt <std::string> O_opt(
        "O",
        cl::desc("Optimization level s, 0-3"),
        cl::Prefix,
        cl::ZeroOrMore,
        cl::cat(FtlCompilerToolCategory));
static cl::opt <std::string> sysroot_opt(
        "sysroot",
        cl::desc("Set the system root directory"),
        cl::Prefix,
        cl::cat(FtlCompilerToolCategory));
static cl::opt <std::string> U_opt(
        "U",
        cl::desc("Undefine macro <macro>"),
        cl::cat(FtlCompilerToolCategory));
static cl::opt<bool> v_opt(
        "v",
        cl::desc("Show commands to run and use verbose output"),
        cl::cat(FtlCompilerToolCategory));
static cl::opt<bool> w_opt(
        "w",
        cl::desc("Suppress all warnings"),
        cl::cat(FtlCompilerToolCategory));
static cl::list <std::string> W_opt(
        "W",
        cl::desc("Enable the specified warning"),
        cl::cat(FtlCompilerToolCategory),
        cl::Prefix,
        cl::ZeroOrMore);
static cl::opt <std::string> contract_name(
        "contract",
        cl::desc("Contract name"),
        cl::cat(FtlCompilerToolCategory));
/// end c/c++ options

/// begin c++ options
#ifdef CPP_COMP
static cl::opt<std::string> std_opt(
    "std",
    cl::desc("Language standard to compile for"),
    cl::cat(FtlCompilerToolCategory));
#endif
/// end c++ options

#endif

struct Options {
    std::string output_fn;
    std::vector <std::string> inputs;
    bool link;
    std::string pp_dir;
    std::string abigen_contract;
    std::vector <std::string> comp_options;
    std::vector <std::string> ld_options;
};

static void GetCompDefaults(std::vector <std::string> &copts) {
    const char *ftl_fixup_suff = "${CMAKE_SHARED_LIBRARY_SUFFIX}";
    std::string apply_lib;
    copts.emplace_back("--target=wasm32");
    copts.emplace_back("-ffreestanding");
    copts.emplace_back("-nostdlib");
    copts.emplace_back("-fno-builtin");

    copts.emplace_back("-fno-threadsafe-statics");
#ifdef CPP_COMP
    copts.emplace_back("-fno-exceptions");
    copts.emplace_back("-fno-rtti");
    copts.emplace_back("-fmodules-ts");
#endif
    copts.emplace_back("-DBOOST_DISABLE_ASSERTS");
    copts.emplace_back("-DBOOST_EXCEPTION_DISABLE");
#ifndef _WIN32
    copts.emplace_back("-Xclang");
    copts.emplace_back("-load");
    copts.emplace_back("-Xclang");
    if (llvm::sys::fs::exists(ftl::whereami::where() + "/LLVMFtlFixup" + ftl_fixup_suff))
        copts.emplace_back(ftl::whereami::where() + "/LLVMFtlFixup" + ftl_fixup_suff);
    else if (llvm::sys::fs::exists(ftl::whereami::where() + "/../lib/LLVMFtlFixup" + ftl_fixup_suff))
        copts.emplace_back(ftl::whereami::where() + "/../lib/LLVMFtlFixup" + ftl_fixup_suff);
    if (llvm::sys::fs::exists(ftl::whereami::where() + "/ftl_plugin" + ftl_fixup_suff))
        copts.emplace_back("-fplugin=" + ftl::whereami::where() + "/ftl_plugin" + ftl_fixup_suff);
    else if (llvm::sys::fs::exists(ftl::whereami::where() + "/../lib/ftl_plugin" + ftl_fixup_suff))
        copts.emplace_back("-fplugin=" + ftl::whereami::where() + "/../lib/ftl_plugin" + ftl_fixup_suff);
#endif
}

#ifdef ONLY_LD
static void GetLdDefaults(std::vector<std::string>& ldopts) {
      ldopts.emplace_back("--gc-sections");
      ldopts.emplace_back("--strip-all");
      ldopts.emplace_back("-zstack-size="+std::string("${FTL_STACK_SIZE}"));
      ldopts.emplace_back("--merge-data-segments");
      ldopts.emplace_back("-e apply");
      ldopts.emplace_back("-lc++ -lc -lftl_malloc");
}
#endif

static Options CreateOptions(bool add_defaults = true) {
    std::string output_fn;
    std::vector <std::string> inputs;
    std::vector <std::string> copts;
    std::vector <std::string> ldopts;
    bool link = true;
    std::string pp_dir;
    std::string abigen_contract;

    if (add_defaults) {
        GetCompDefaults(copts);
#ifdef ONLY_LD
        GetLdDefaults(ldopts);
#endif
    }

    pp_dir = ftl::whereami::where();

    copts.emplace_back("-mllvm");
    copts.emplace_back("-use-cfl-aa-in-codegen=both");
#ifdef ONLY_LD
    ldopts.emplace_back("-mllvm");
    ldopts.emplace_back("-use-cfl-aa-in-codegen=both");
#endif

    for (auto input_filename : input_filename_opt) {
#ifdef ONLY_LD
        ldopts.push_back(input_filename);
#else
        inputs.push_back(input_filename);
#endif
    }

    ldopts.emplace_back("-L" + ftl::whereami::where() + "/../lib");

#ifndef ONLY_LD
    copts.emplace_back("-I" + ftl::whereami::where() + "/../include/libcxx");
    copts.emplace_back("-I" + ftl::whereami::where() + "/../include/libc");
    copts.emplace_back("-I" + ftl::whereami::where() + "/../include");

    copts.emplace_back("-c");
    if (c_opt) {
        link = false;
    }

    for (auto define : D_opt) {
        copts.emplace_back("-D" + define);
    }
    for (auto include : include_opt) {
        copts.emplace_back("-include=" + include);
    }
    for (auto inc_dir : I_opt) {
        copts.emplace_back("-I" + inc_dir);
    }
    if (O_opt.empty()) {
        copts.emplace_back("-O3");
    } else if (!O_opt.empty()) {
        copts.emplace_back("-O" + O_opt);
    }
    if (v_opt) {
        copts.emplace_back("-v");
    }
    if (w_opt) {
        copts.emplace_back("-w");
    }
    for (auto warn : W_opt) {
        copts.emplace_back("-W" + warn);
    }

#endif

#ifdef ONLY_LD
    ldopts.emplace_back("-stack-first");
#endif

    for (auto lib_dir : L_opt) {
        ldopts.emplace_back("-L" + lib_dir);
    }
    for (auto library : l_opt) {
        ldopts.emplace_back("-l" + library);
    }
    if (o_opt.empty()) {
#ifndef ONLY_LD
        if (inputs.size() == 1) {
            llvm::SmallString<256> fn = llvm::sys::path::filename(inputs[0]);
            llvm::SmallString<256> fn2 = fn;
            llvm::sys::path::replace_extension(fn, ".wasm");
            output_fn = fn.str();
            llvm::SmallString<256> res;
            llvm::sys::path::system_temp_directory(true, res);
            ldopts.emplace_back(std::string(std::string(res.str()) + "/" + std::string(fn2.str()) + ".o"));
        } else {
            ldopts.emplace_back("a.out");
        }
#endif
        if (inputs.size() == 1) {
            llvm::SmallString<256> fn = llvm::sys::path::filename(inputs[0]);
            llvm::sys::path::replace_extension(fn, ".wasm");
            ldopts.emplace_back("-o " + output_fn);
            output_fn = fn.str();
        } else {
            ldopts.emplace_back("-o a.out");
            output_fn = "a.out";
        }
    } else {
        ldopts.emplace_back("-o " + o_opt);
        output_fn = o_opt;
    }

#ifdef ONLY_LD
    ldopts.emplace_back("--allow-undefined-file="+ftl::whereami::where()+"/../lib/ftl.imports");
#endif

#ifndef ONLY_LD
#ifdef CPP_COMP
    if (! std_opt.empty()) {
       copts.emplace_back("--std="+std_opt);
    } else {
       copts.emplace_back("--std=c++17");
    }
#endif
    if (!contract_name.empty())
        abigen_contract = contract_name;
    else {
        llvm::SmallString<256> fn = llvm::sys::path::filename(output_fn);
        llvm::sys::path::replace_extension(fn, "");
        abigen_contract = fn.str();
    }
#endif

    return {output_fn, inputs, link, pp_dir, abigen_contract, copts, ldopts};
}
