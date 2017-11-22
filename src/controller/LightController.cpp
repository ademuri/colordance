#include "LightController.hpp"
#include "ParamController.hpp"
#include <cstdio>
#include <vector>

std::vector<std::vector<uint16_t>>
LightController::GetLights(ParamController *paramController, int16_t rows,
                           int16_t cols) {
  std::vector<std::vector<uint16_t>> selectedLights;

  const int16_t numRows = lightIds.size();
  const int16_t numCols = lightIds[0].size();

  // Check that rows and cols aren't too big. Note that all cols should be the
  // same size.
  // TODO: do something different in production?
  if (rows > numRows || cols > numCols) {
    printf("Rows or cols too big in GetLights: requested (%u, %u), present "
           "(%d, %d).\n",
           rows, cols, numRows, numCols);
    rows = numRows;
    cols = numCols;
  }

  int16_t lowerRow;
  int16_t upperRow;
  int16_t lowerCol;
  int16_t upperCol;

  int16_t excessHighRows = numRows - (rows / 2 + centerLightRow);
  if (excessHighRows < 0) {
    upperRow = numRows - 1;
  } else {
    upperRow = rows / 2 + centerLightRow;

    // Even numbers aren't symmetrical around the center.
    if (rows % 2 == 0) {
      upperRow--;
    }
  }

  int16_t excessLowRows = centerLightRow - rows / 2;
  if (excessLowRows < 0) {
    lowerRow = 0;
    upperRow -= excessLowRows;
  } else {
    lowerRow = excessLowRows;
    if (excessHighRows < 0) {
      lowerRow += excessHighRows;
    }
  }

  int16_t excessHighCols = numCols - (cols / 2 + centerLightCol);
  if (excessHighCols < 0) {
    upperCol = numCols - 1;
  } else {
    upperCol = cols / 2 + centerLightCol;

    // Even numbers aren't symmetrical around the center.
    if (cols % 2 == 0) {
      upperCol--;
    }
  }

  int16_t excessLowCols = centerLightCol - cols / 2;
  if (excessLowCols < 0) {
    lowerCol = 0;
    upperCol -= excessLowCols;
  } else {
    lowerCol = excessLowCols;
    if (excessHighCols < 0) {
      lowerCol += excessHighCols;
    }
  }

  for (uint16_t i = lowerRow; i <= upperRow; i++) {
    std::vector<uint16_t> row;
    for (uint16_t j = lowerCol; j <= upperCol; j++) {
      row.push_back(lightIds[i][j]);
    }
    selectedLights.push_back(row);
  }

  return selectedLights;
}
