#ifndef HYPE_PROFILE_H
#define HYPE_PROFILE_H

#include <boost/container/vector.hpp>
#include <boost/unordered_map.hpp>
#include <string>

typedef struct ValueType_s {
    // Type and uint do not present in proto file
    // Used only for parsing
    // cpu, wall, inuse_space, etc
    std::string type;
    // seconds, nanoseconds, bytes, etc
    std::string unit;

    // index in the string table
    int64_t type_index;
    // index in the string table
    int64_t unit_index;
} ValueType_t;

// mapping corresponds to Profile.Mapping
typedef struct Mapping_s {
    // Unique nonzero id for the mapping.
    uint64_t id;
    // Address at which the binary (or DLL) is loaded into memory.
    uint64_t memory_start;
    // The limit of the address range occupied by this mapping.
    uint64_t memory_limit;
    // Offset in the binary that corresponds to the first mapped address.
    uint64_t memory_offset;
    // Index into string table
    // The object this entry is loaded from.  This can be a filename on
    // disk for the main binary and shared libraries, or virtual
    // abstractions like "[vdso]".
    std::string filename;
    // Index into string table
    // A string that uniquely identifies a particular program version
    // with high probability. E.g., for binaries generated by GNU tools,
    // it could be the contents of the .note.gnu.build-id field.
    std::string build_id;
    bool has_function;
    bool has_filenames;
    bool has_line_numbers;
    bool has_inline_frames;

    // Index into string table
    int64_t filename_index;
    // Index into string table
    int64_t build_id_index;
} Mapping_t;

typedef struct Label_s {
    // Index into string table
    int64_t key_index;

    // one of the two following values must be set
    int64_t str_index;
    // Index into string table
    int64_t num_index;

    // Should only be present when num is present.
    // Specifies the units of num.
    // Use arbitrary string (for example, "requests") as a custom count unit.
    // If no unit is specified, consumer may apply heuristic to deduce the unit.
    // Consumers may also  interpret units like "bytes" and "kilobytes" as memory
    // units and units like "seconds" and "nanoseconds" as time units,
    // and apply appropriate unit conversions to these.
    int64_t num_unit_index;
} Label_t;

typedef struct Function_s {
    // Unique nonzero id for the function.
    uint64_t id;
    // Name of the function, in human-readable form if available.
    std::string name;
    // Name of the function, as identified by the system.
    // For instance, it can be a C++ mangled name.
    std::string system_name;
    // Source file containing the function.
    std::string filename;
    // Line number in source file.
    int64_t start_line;

    // HELPERS
    // Index into string table
    int64_t name_index;
    // Index into string table
    int64_t system_name_index;
    // Index into string table
    int64_t filename_index;
} Function_t;

typedef struct Line_s {
    // Line number in source code.
    int64_t line{};
    // The id of the corresponding profile.Function for this line.
    uint64_t function_index{};

    // HELPERS
    Function_t function;
} Line_t;

// Describes function and line table debug information.
typedef struct Location_s {
    // Unique nonzero id for the location.  A profile could use
    // instruction addresses or any integer sequence as ids.
    uint64_t id;
    // The id of the corresponding profile.Mapping for this location.
    // It can be unset if the mapping is unknown or not applicable for
    // this profile type.
    uint64_t mapping_index;
    // The instruction address for this location, if available.  It
    // should be within [Mapping.memory_start...Mapping.memory_limit]
    // for the corresponding mapping. A non-leaf address may be in the
    // middle of a call instruction. It is up to display tools to find
    // the beginning of the instruction if necessary.
    uint64_t address;
    // Multiple line indicates this location has inlined functions,
    // where the last entry represents the caller into which the
    // preceding entries were inlined.
    //
    // E.g., if memcpy() is inlined into printf:
    //    line[0].function_name == "memcpy"
    //    line[1].function_name == "printf"
    boost::container::vector<Line_t> line;
    // Provides an indication that multiple symbols map to this location's
    // address, for example due to identical code folding by the linker. In that
    // case the line information above represents one of the multiple
    // symbols. This field must be recomputed when the symbolization state of the
    // profile changes.
    bool is_folder;

    //HELPER
    Mapping_t mapping;
} Location_t;

typedef struct Sample_s {
    boost::container::vector<Location_t> location;
    boost::container::vector<int64_t> value;

    boost::unordered_map<std::string, boost::container::vector<std::string>> label;
    boost::unordered_map<std::string, boost::container::vector<int64_t>> num_label;
    boost::unordered_map<std::string, boost::container::vector<std::string>> num_unit_label;

    boost::container::vector<uint64_t> location_index;
    boost::container::vector<Label_

} Sample_t;

typedef struct Profile_s {
    boost::container::vector<int> sample_type;


} Profile_t;

#endif //HYPE_PROFILE_H
























































