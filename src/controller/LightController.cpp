#include "LightController.hpp"
#include <cmath>
#include <cstdio>
#include <vector>
#include "ParamController.hpp"

//#define DEBUG_LIGHT_CHOICE

std::vector<std::vector<int16_t>> LightController::GetLights(
    ParamController *paramController, int16_t rows, int16_t cols) {
  std::vector<std::vector<int16_t>> selectedLights;

  const int16_t numRows = lightIds.size();
  const int16_t numCols = lightIds[0].size();

  // Check that rows and cols aren't too big. Note that all cols should be the
  // same size.
  // TODO: do something different in production?
  if (rows > numRows || cols > numCols) {
#ifndef ARDUINO
    // TODO: use a better solution than ifndef
    printf(
        "Rows or cols too big in GetLights: requested (%u, %u), present "
        "(%d, %d).\n",
        rows, cols, numRows, numCols);
#endif
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

  // Pan the lights to the left or right, based on the kPan param.
  // This is the amount that we can shift left or right
  const int16_t possiblePan = lowerCol + (numCols - upperCol);
  const int16_t totalPan =
      paramController->GetScaled(Params::kPan, 0, possiblePan);

  if (totalPan < lowerCol) {
    upperCol = upperCol - lowerCol + totalPan;
    lowerCol = totalPan;
  } else if (totalPan > lowerCol) {
    upperCol += (totalPan - lowerCol) - 1;
    lowerCol += (totalPan - lowerCol) - 1;
  }

  // Tilt the lights up or down, based on the kTilt param. This is the amount
  // that we can shift up or down.
  const int16_t possibleTilt = lowerRow + (numRows - upperRow);
  const int16_t totalTilt =
      paramController->GetScaled(Params::kTilt, 0, possibleTilt);

  if (totalTilt < lowerRow) {
    upperRow = upperRow - lowerRow + totalTilt;
    lowerRow = totalTilt;
  } else if (totalTilt > lowerRow) {
    upperRow += (totalTilt - lowerRow) - 1;
    lowerRow += (totalTilt - lowerRow) - 1;
  }

  for (int16_t i = lowerRow; i <= upperRow; i++) {
    std::vector<int16_t> row;
    for (int16_t j = lowerCol; j <= upperCol; j++) {
      row.push_back(lightIds[i][j]);
    }
    selectedLights.push_back(row);
  }

#ifdef DEBUG_LIGHT_CHOICE
  printf("Selected lights: {");
  for (int16_t i = 0; i < selectedLights.size(); i++) {
    printf(" {");
    for (int16_t j = 0; j < selectedLights[i].size() - 1; j++) {
      printf("%d, ", selectedLights[i][j]);
    }
    printf("%d}, ", selectedLights[i][selectedLights[i].size() - 1]);
  }
  printf("}\n");
#endif

  return selectedLights;
}

std::vector<int16_t> LightController::GetLightsFromParams(
    ParamController *paramController) {
  return GetLightsFromParams(paramController, 0);
}

std::vector<int16_t> LightController::GetLightsFromParams(
    ParamController *paramController, unsigned int width) {
  std::vector<int16_t> availableLights;
  const int16_t orientation =
      paramController->GetScaled(Params::kOrientation, 0, 2);
  int16_t tilt;

  // Flip pan and tilt if orientation is vertical, so that they still track the
  // physical direction
  Params panParam = Params::kPan;
  Params tiltParam = Params::kTilt;

  // Depending on the orientation, choose lights in a vertical stack instead of
  // horizontally. If vertical, pan and tilt are flipped.
  switch (orientation) {
    case 0:
      // Horizontal (i.e. "normal")
      tilt = paramController->GetScaled(tiltParam, 0, numRows - 1);

      for (int i = 0; i < lightIds[tilt].size(); i++) {
        if (lightIds[tilt][i] != 0) {
          availableLights.push_back(lightIds[tilt][i]);
        }
      }
      break;

    case 1:
      // Vertical (tilt and pan are flipped from normal)
      panParam = Params::kTilt;
      tiltParam = Params::kPan;

      tilt = paramController->GetScaled(tiltParam, 0, numCols - 1);

      for (int i = 0; i < lightIds.size(); i++) {
        if (lightIds[i][tilt] != 0) {
          availableLights.push_back(lightIds[i][tilt]);
        }
      }
      break;

    case 2: {
      // Diagonal
      unsigned int diagonalSize = std::min(lightIds.size(), lightIds[0].size());
      for (unsigned int i = 0; i < diagonalSize; i++) {
        availableLights.push_back(lightIds[i][i]);
      }
      break;
    }

    default:
#ifndef ARDUINO
      printf("Warning: hit unexpected default case in GetLightsFromParams\n");
#endif
      break;
  }

  int16_t numLights =
      paramController->GetScaled(Params::kWidth, 1, availableLights.size());
  if (width != 0) {
    // Can't use std::min here because vector size type varies across platforms
    if (availableLights.size() < width) {
      numLights = availableLights.size();
    } else {
      numLights = width;
    }
  }
  const int16_t pan = paramController->GetScaled(
      panParam, 0, availableLights.size() - numLights);
  std::vector<int16_t> chosenLights;
  for (int i = 0; i < numLights; i++) {
    chosenLights.push_back(availableLights[i + pan]);
  }

  return chosenLights;
}

void LightController::Blackout() {
  for (auto rowIter : lightIds) {
    for (auto colIter : rowIter) {
      Set(colIter, {0, 0, 0});
    }
  }
}
