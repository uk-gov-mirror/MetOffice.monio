/******************************************************************************
* MONIO - Met Office NetCDF Input Output                                      *
*                                                                             *
* (C) Crown Copyright 2025, Met Office. All rights reserved.                  *
*                                                                             *
* This software is licensed under the terms of the 3-Clause BSD License       *
* which can be obtained from https://opensource.org/license/bsd-3-clause/.    *
******************************************************************************/
#pragma once

#define ECKIT_TESTING_SELF_REGISTER_CASES 0

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include <mpi.h>

#include "atlas/functionspace/CubedSphereColumns.h"
#include "atlas/grid/CubedSphereGrid.h"
#include "atlas/grid/Iterator.h"
#include "atlas/mesh/Mesh.h"
#include "atlas/meshgenerator/MeshGenerator.h"

#include "eckit/filesystem/LocalPathName.h"
#include "eckit/testing/Test.h"

#include "monio/Constants.h"
#include "monio/FileData.h"
#include "monio/Metadata.h"
#include "monio/Reader.h"
#include "monio/Writer.h"

#include "oops/../test/TestEnvironment.h"
#include "oops/mpi/mpi.h"
#include "oops/runs/Test.h"
#include "oops/util/Logger.h"

namespace monio {
namespace test {

void testFunction() {
  oops::Log::info() << "basicWriteTest()" << std::endl;

  //const eckit::LocalConfiguration inputConfig(::test::TestEnvironment::config(), "filePaths");
  //eckit::LocalPathName inputFilePath  = inputConfig.getString("inputFilePath");
  //eckit::LocalPathName outputFilePath = inputConfig.getString("outputFilePath");

  oops::Log::info() << oops::mpi::myself().size() << std::endl;

  
}

class MPIRead : public oops::Test{
 public:
  MPIRead() {}
  virtual ~MPIRead() {}
 private:
  std::string testid() const override {
    return "monio::test::MPIRead";
  }

  void register_tests() const override {
    std::vector<eckit::testing::Test>& ts = eckit::testing::specification();

    std::function<void(std::string&, int&, int)> testFn =
        [](std::string &, int&, int) { testFunction(); };
    ts.push_back(eckit::testing::Test("monio/test_mpi_read", testFn));
  }
  void clear() const override {}
};
}  // namespace test
}  // namespace monio
