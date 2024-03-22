#include "seqio.h"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <memory>
#include <algorithm>
#include <optional>
#include <pybind11/cast.h>
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <string>

namespace py = pybind11;

class seqioRecordImpl {

public:
  seqioRecordImpl(seqioString* name,
                  seqioString* comment,
                  seqioString* sequence,
                  seqioString* quality)
  {
    this->name = std::string();
    this->name.append(name->data, name->length);
    this->sequence = std::string();
    this->sequence.append(sequence->data, sequence->length);
    if (comment != nullptr) {
      this->comment = std::string();
      this->comment.append(comment->data, comment->length);
    }
    if (quality != nullptr) {
      this->quality = std::string();
      this->quality.append(quality->data, quality->length);
    }
  }
  ~seqioRecordImpl() {}
  std::string
  get_name()
  {
    return name;
  }
  std::string
  get_sequence()
  {
    return sequence;
  }
  std::string
  get_quality()
  {
    return quality;
  }
  std::string
  get_comment()
  {
    return comment;
  }

  std::string
  upper()
  {
    using namespace std;
    std::string upper_sequence = sequence;
    std::transform(upper_sequence.begin(), upper_sequence.end(),
                   upper_sequence.begin(), ::toupper);
    return upper_sequence;
  }

  std::string
  lower()
  {
    using namespace std;
    std::string lower_sequence = sequence;
    std::transform(lower_sequence.begin(), lower_sequence.end(),
                   lower_sequence.begin(), ::tolower);
    return lower_sequence;
  }

  size_t
  length()
  {
    return sequence.length();
  }

private:
  std::string name;
  std::string comment;
  std::string sequence;
  std::string quality;
};

class seqioFileImpl {
public:
  std::string filename;
  seqOpenMode mode;
  bool isGzipped;

  seqioFileImpl(std::string filename, seqOpenMode mode, bool isGzipped)
  {
    this->filename = filename;
    this->mode = mode;
    this->isGzipped = isGzipped;
    this->openOptions = seqioOpenOptions();
    this->writeOptions = seqioWriteOptions();
    this->openOptions.filename = filename.c_str();
    this->openOptions.mode = mode;
    this->openOptions.isGzipped = isGzipped;
    this->file = seqioOpen(&openOptions);
    this->record = NULL;
  }

  ~seqioFileImpl() { seqioClose(file); }

  std::shared_ptr<seqioRecordImpl>
  readOne()
  {
    this->record = seqioRead(file, this->record);
    if (this->record == NULL) {
      return NULL;
    }
    return std::make_shared<seqioRecordImpl>(
        this->record->name, this->record->comment, this->record->sequence,
        this->record->quality);
  }

  std::shared_ptr<seqioRecordImpl>
  readFasta()
  {
    if (file->pravite.type != seqioRecordTypeFasta) {
      fprintf(stderr, "Error: file is not fasta file\n");
      return NULL;
    }
    this->record = seqioReadFasta(file, this->record);
    if (this->record == NULL) {
      return NULL;
    }
    return std::make_shared<seqioRecordImpl>(
        this->record->name, this->record->comment, this->record->sequence,
        this->record->quality);
  }
  std::shared_ptr<seqioRecordImpl>
  readFastq()
  {
    if (file->pravite.type != seqioRecordTypeFastq) {
      fprintf(stderr, "Error: file is not fastq file\n");
      return NULL;
    }
    this->record = seqioReadFastq(file, this->record);
    if (this->record == NULL) {
      return NULL;
    }
    return std::make_shared<seqioRecordImpl>(
        this->record->name, this->record->comment, this->record->sequence,
        this->record->quality);
  }

private:
  seqioFile* file;
  seqioOpenOptions openOptions;
  seqioWriteOptions writeOptions;
  seqioRecord* record;
};

PYBIND11_MODULE(_seqio, m)
{
  py::enum_<seqOpenMode>(m, "seqOpenMode")
      .value("READ", seqOpenMode::seqOpenModeRead)
      .value("WRITE", seqOpenMode::seqOpenModeWrite)
      .export_values();

  py::class_<seqioRecordImpl, std::shared_ptr<seqioRecordImpl> >(m,
                                                                 "seqioRecord")
      .def(py::init<seqioString*, seqioString*, seqioString*, seqioString*>())
      .def_property_readonly("name", &seqioRecordImpl::get_name)
      .def_property_readonly("sequence", &seqioRecordImpl::get_sequence)
      .def_property_readonly("quality", &seqioRecordImpl::get_quality)
      .def_property_readonly("comment", &seqioRecordImpl::get_comment)
      .def("upper", &seqioRecordImpl::upper)
      .def("lower", &seqioRecordImpl::lower)
      .def("length", &seqioRecordImpl::length);

  py::class_<seqioFileImpl, std::shared_ptr<seqioFileImpl> >(m, "seqioFile")
      .def(py::init<std::string, seqOpenMode, bool>())
      .def("readOne", &seqioFileImpl::readOne)
      .def("readFasta", &seqioFileImpl::readFasta)
      .def("readFastq", &seqioFileImpl::readFastq);
}
