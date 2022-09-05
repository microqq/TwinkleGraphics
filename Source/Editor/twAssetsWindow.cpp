
#include <algorithm>
#include <sstream>
#include <sys/stat.h>

#include <functional>

#include "twAssetsWindow.h"
#include "twCustomFont_1.h"

#ifndef DateTimeFormat
#define DateTimeFormat "%Y/%m/%d %H:%M"
#endif // DateTimeFormat

namespace TwinkleGraphics {
using namespace std::string_literals;

using FatKey = AssetInfo;
using LightKey = std::string;

bool operator<(const FatKey &fk, const LightKey &lk) {
  return fk.path.compare(lk) < 0;
}
bool operator<(const LightKey &lk, const FatKey &fk) {
  return lk.compare(fk.path) < 0;
}
bool operator<(const FatKey &fk1, const FatKey &fk2) {
  return fk1.path.compare(fk2.path) < 0;
}

AssetStyle::AssetStyle() {}

AssetStyle::AssetStyle(const AssetStyle &style)
    : color(style.color), icon(style.icon), font(style.font) {}

AssetStyle::AssetStyle(const ImVec4 &col, const std::string &ico, ImFont *ft)
    : color(col), icon(ico), font(ft) {}

AssetInfo::AssetInfo(const fs::path &assetPath, AssetType assetType,
                     bool assetSelected)
    : type(assetType), selected(assetSelected) {
  path = assetPath.string();
  hash = std::hash<std::string>{}(path);
  baseName = assetPath.stem().string();
  extension = assetPath.extension().string();

  if (!fs::is_directory(assetPath)) {
    // update file size
    int i{};
    double mantissa = fs::file_size(assetPath);
    fileSize = mantissa;
    for (; mantissa >= 1024.; mantissa /= 1024., ++i) {
    }
    mantissa = std::ceil(mantissa * 10.) / 10.;

    std::ostringstream streamObj;
    streamObj << std::setprecision(3);
    streamObj << mantissa;

    fileSizeString = streamObj.str() + "BKMGTPE"[i] + (i == 0 ? "" : "B");

    // update file last write time
    struct stat statInfos = {};
    char timebuf[100];
    int result = stat(path.c_str(), &statInfos);
    if (!result) {
      size_t len = 0;
#ifdef _MSC_VER
      struct tm tm;
      errno_t err = localtime_s(&tm, &statInfos.st_mtime);
      if (!err)
        len = strftime(timebuf, 99, DateTimeFormat, &tm);
#else  // _MSC_VER
      struct tm *tm = localtime(&statInfos.st_mtime);
      if (tm)
        len = strftime(timebuf, 99, DateTimeFormat, tm);
#endif // _MSC_VER
      if (len) {
        fileModifyDate = std::string(timebuf, len);
      }
    }
  }
}

AssetsWindow::AssetsWindow(const std::string &name, uint32 width, uint32 height,
                           const std::string &assetsDir, Widget *parent)
    : Window(name, width, height, parent), _assetsDir(assetsDir) {
  this->SetAssetStyle(AssetStyleFlags_BY_TYPE_EXTENSION, ".png",
                      ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_IGFD_FILE_PIC);
  this->SetAssetStyle(AssetStyleFlags_BY_TYPE_EXTENSION, ".bmp",
                      ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_IGFD_FILE_PIC);
  this->SetAssetStyle(AssetStyleFlags_BY_TYPE_EXTENSION, ".BMP",
                      ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_IGFD_FILE_PIC);
  this->SetAssetStyle(AssetStyleFlags_BY_TYPE_EXTENSION, ".dds",
                      ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_IGFD_FILE_PIC);
  this->SetAssetStyle(AssetStyleFlags_BY_TYPE_EXTENSION, ".tga",
                      ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_IGFD_FILE_PIC);
  this->SetAssetStyle(AssetStyleFlags_BY_TYPE_EXTENSION, ".jpg",
                      ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_IGFD_FILE_PIC);
  this->SetAssetStyle(AssetStyleFlags_BY_TYPE_EXTENSION, ".JPG",
                      ImVec4(0.7f, 0.7f, 0.7f, 1.0f), ICON_IGFD_FILE_PIC);
}

AssetsWindow::~AssetsWindow() {}

void AssetsWindow::Update(float deltaTime) {}

void AssetsWindow::SetAssetStyle(const AssetStyleFlags &flags,
                                 const char *criteria,
                                 const AssetStyle &style) {
  std::string sCriteria;
  if (criteria) {
    sCriteria = std::string(criteria);
  }
  _assetsStyle[flags][sCriteria] = std::make_shared<AssetStyle>(style);
}

void AssetsWindow::SetAssetStyle(const AssetStyleFlags &flags,
                                 const char *criteria, const ImVec4 &color,
                                 const std::string &icon, ImFont *font) {
  std::string sCriteria;
  if (criteria) {
    sCriteria = std::string(criteria);
  }
  _assetsStyle[flags][sCriteria] =
      std::make_shared<AssetStyle>(color, icon, font);
}

std::shared_ptr<AssetStyle>
AssetsWindow::GetAssetStyle(const AssetStyleFlags &flags,
                            const char *criteria) {
  auto iterStyleByFlags = _assetsStyle.find(flags);
  if (iterStyleByFlags != _assetsStyle.end()) {
    auto iterAssetStyle = iterStyleByFlags->second.find(criteria);
    if (iterAssetStyle != iterStyleByFlags->second.end()) {
      return iterAssetStyle->second;
    }
  }

  return nullptr;
}

void AssetsWindow::OnGuiBegin() {
  // ImGui::SetNextWindowSize(ImVec2(_pData->width, _pData->height));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin(_name.c_str(), NULL,
               ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoScrollWithMouse);
}

void AssetsWindow::OnGui() {
  ImGui::Dummy(ImVec2(0.0f, 1.0f));

  ImGui::PushItemWidth(200);
  if (ImGui::InputTextWithHint("##search", ICON_IGFD_SEARCH " Search",
                               _searchFilter, sizeof(_searchFilter),
                               ImGuiInputTextFlags_EnterReturnsTrue)) {
  }
  ImGui::PopItemWidth();

  ImGui::SameLine();
  ImGui::PushItemWidth(200);
  ImGui::SliderFloat("Scale", &_thumbnailSize, 0.2f, 3.f, "%.2f",
                     ImGuiSliderFlags_AlwaysClamp);
  ImGui::PopItemWidth();

  ImGui::SameLine();
  ImGui::Selectable(ICON_IGFD_THUMBAILS_LIST, &_thumbnailsList,
                    ImGuiSelectableFlags_None, ImVec2(15, 18));

  ImGui::SameLine();
  if (ImGui::Selectable(ICON_IGFD_SMALL_THUMBAILS, !_thumbnailsList,
                        ImGuiSelectableFlags_None, ImVec2(15, 18))) {
    _thumbnailsList = false;
  }

  ImGui::Separator();

  ImVec2 contentSize = ImGui::GetContentRegionAvail();
  ImGui::Columns(2);
  // float columnWidth = ImGui::GetColumnWidth();
  static bool setColumnWidth = false;
  if (!setColumnWidth) {
    ImGui::SetColumnWidth(0, contentSize.x * 0.2f);
    ImGui::SetColumnWidth(1, contentSize.x * 0.8f);
    setColumnWidth = true;
  }

  // subwindow Project Tree
  ImGui::BeginChild(u8"Project Folder",
                    ImVec2(contentSize.x * 0.2f, contentSize.y), false,
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
  if (UpdateTreeView()) {
    UpdatePreviewAssets();
  }
  ImGui::EndChild();

  // subwindow Assets Preview
  ImGui::NextColumn();
  // columnWidth = ImGui::GetColumnWidth();
  ImGui::BeginChild(u8"Assets Preview",
                    ImVec2(contentSize.x * 0.8f, contentSize.y), false,
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
  UpdatePreview();
  ImGui::EndChild();
}

void AssetsWindow::OnGuiEnd() {
  SetFocusedInternal();
  SetHoveredInternal();

  ImGui::End();
  ImGui::PopStyleVar();
}

void AssetsWindow::Destroy() {
  Window::Destroy();

  _assets.clear();
  _assetsStyle.clear();
  _selectedAssets.clear();
  _previewAssets.clear();
}

void AssetsWindow::SetFocusedInternal() {
  bool focused = ImGui::IsWindowFocused(ImGuiHoveredFlags_RootAndChildWindows);
  if (_focused != focused) {
    _focused = focused;
    if (_focused) {
      // focusedin
    } else {
      // focusedout
    }
    // Console::LogInfo("Assets window focused:", _focused, "\n");
  }
}

void AssetsWindow::SetHoveredInternal() {
  bool hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
  if (_hovered != hovered) {
    _hovered = hovered;
    if (_hovered) {

    } else {
      /* code */
    }
    // Console::LogInfo("Assets window hovered:", _hovered, "\n");
  }
}

bool AssetsWindow::UpdateTreeView() {
  if (!std::filesystem::exists(_assetsDir)) {
    return false;
  }

  auto path = fs::path(_assetsDir);
  _assets.emplace(std::make_pair(
      path.string(), AssetInfo(path, AssetType::AssetType_FOLDER, false)));

  std::string lastSelectedItem = _treeSelectedAsset;
  CreateFolderTreeNode(fs::path(_assetsDir));
  return !_treeSelectedAsset.empty() &&
         lastSelectedItem.compare(_treeSelectedAsset) != 0;
}

void AssetsWindow::CreateFolderTreeNode(const fs::path &path) {
  ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;

  // node selected tag
  if (_treeSelectedAsset.compare(path.string()) == 0) {
    nodeFlags |= ImGuiTreeNodeFlags_Selected;
  }

  // tree node label
  std::string label = ICON_IGFD_FOLDER + "  "s + path.filename().string();
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.75f, 0.2f, 1.0f));
  bool open = ImGui::TreeNodeEx(label.c_str(), nodeFlags);
  if (open || ImGui::IsItemClicked()) {
    if (ImGui::IsItemClicked()) {
      _treeSelectedAsset = path.string();
    }

    AssetType type;
    for (auto const &entry : fs::directory_iterator{path}) {
      if (fs::is_directory(entry.symlink_status())) {
        type = AssetType::AssetType_FOLDER;
      } else {
        type = AssetType::AssetType_FILE;
      }
      _assets.emplace(std::make_pair(entry.path().string(),
                                     AssetInfo(entry.path(), type, false)));
    }
  }
  if (open) {
    std::vector<std::string> entryPaths;
    for (auto const &entry : fs::directory_iterator{path}) {
      if (fs::is_directory(entry.symlink_status())) {
        CreateFolderTreeNode(entry.path());
      } else {
        entryPaths.emplace_back(entry.path().string());
      }
    }

    for (auto const &entryPath : entryPaths) {
      fs::path entry = entryPath;

      std::string icon = ICON_IGFD_FILE;
      auto iterAsset = _assets.find(entryPath);
      if (iterAsset != _assets.end()) {
        auto assetStyle =
            this->GetAssetStyle(AssetStyleFlags_BY_TYPE_EXTENSION,
                                iterAsset->second.extension.c_str());
        if (assetStyle != nullptr) {
          icon = assetStyle->icon;
        }
      }

      std::string entryLabel = icon + "  "s + entry.filename().string();

      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
      if (ImGui::Selectable(entryLabel.c_str(),
                            entryPath.compare(_treeSelectedAsset) == 0)) {
        _treeSelectedAsset = entryPath;
      }
      ImGui::PopStyleColor();
    }
    entryPaths.clear();
    ImGui::TreePop();
  }
  ImGui::PopStyleColor();
}

void AssetsWindow::UpdatePreviewAssets() {
  _previewAssets.clear();
  if (fs::is_directory(fs::status(_treeSelectedAsset))) {
    for (auto const &entry :
         fs::directory_iterator{fs::path(_treeSelectedAsset)}) {

      auto iter = _assets.find(entry.path().string());
      if (iter != _assets.end()) {
        _previewAssets.emplace_back(&(iter->second));
      }
    }
  }
  SortPreviewAssets(SortTag::EXTENSION);
}

void AssetsWindow::SortPreviewAssets(SortTag tag) {
  std::sort(_previewAssets.begin(), _previewAssets.end(),
            [&](AssetInfo *a, AssetInfo *b) {
              bool ret = true;
              int compare = 0;
              switch (tag) {
              case SortTag::FILENAME:
                ret = a->baseName.compare(b->baseName) < 0;
                break;
              case SortTag::EXTENSION:
                compare = a->extension.compare(b->extension);
                ret = compare == 0 ? a->baseName.compare(b->baseName) < 0
                                   : compare < 0;
                break;
              case SortTag::SIZE:
                ret = a->fileSize > b->fileSize;
                if (!ret && a->fileSize == b->fileSize) {
                  ret = a->baseName.compare(b->baseName) < 0;
                }
                break;
              case SortTag::DATE:
                compare = a->fileModifyDate.compare(b->fileModifyDate) < 0;
                ret = compare == 0 ? a->baseName.compare(b->baseName) < 0
                                   : compare < 0;
                break;
              default:
                break;
              }
              return ret;
            });
}

void AssetsWindow::UpdatePreview() {
  auto assetsCount = _previewAssets.size();
  if (assetsCount == 0) {
    return;
  }

  float w = ImGui::GetContentRegionAvail().x;
  int columns = !_thumbnailsList ? (int)w / int(TILE_SIZE * _thumbnailSize) : 4;
  columns = std::max(columns, 1);
  auto rowCount =
      !_thumbnailsList ? (assetsCount + columns - 1) / columns : assetsCount;

  ImGui::Columns(columns);
  if (_thumbnailsList) {
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "File Name");
    ImGui::NextColumn();
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Type");
    ImGui::NextColumn();
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Size");
    ImGui::NextColumn();
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Date");
  }

  ImGuiListClipper clipper;
  clipper.Begin((int)rowCount);
  while (clipper.Step()) {
    for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
      if (_thumbnailsList) {
        auto asset = _previewAssets[i];
        std::string label;
        if (asset->type == AssetType::AssetType_FOLDER) {
          label = ICON_IGFD_FOLDER + " "s +
                  fs::path(asset->path).filename().string();
          ImGui::PushStyleColor(ImGuiCol_Text,
                                ImVec4(0.95f, 0.75f, 0.2f, 1.0f));
        } else {
          std::string icon = ICON_IGFD_FILE;
          auto assetStyle = this->GetAssetStyle(
              AssetStyleFlags_BY_TYPE_EXTENSION, asset->extension.c_str());
          if (assetStyle != nullptr) {
            icon = assetStyle->icon;
          }

          label = icon + " "s + fs::path(asset->path).filename().string();
          ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
        }
        ImGui::NextColumn();
        ImGui::Selectable(label.c_str(), &(asset->selected),
                          ImGuiSelectableFlags_None);
        ImGui::PopStyleColor();

        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s",
                           asset->extension.c_str());
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s",
                           asset->fileSizeString.c_str());
        ImGui::NextColumn();
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s",
                           asset->fileModifyDate.c_str());
      } else {
      }
    }
  }
  ImGui::Columns(1);
}

} // namespace TwinkleGraphics
