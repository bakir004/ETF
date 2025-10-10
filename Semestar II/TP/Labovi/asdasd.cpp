#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>

class MediaFile {
protected:
  std::string naziv;
  int trajanje;
public:
  MediaFile(std::string naziv, int trajanje);
  virtual void PrintInfo() const = 0;
  virtual void Play() const = 0;
  virtual ~MediaFile() {}
};

class AudioFile : public MediaFile {
  int bitrate;
public:
  void Play() const override {
    std::cout << "Playing audio file: " << naziv << " with bitrate: " << bitrate << " kbps\n";
  }
  void PrintInfo() const override {
    std::cout << "Audio File: " << naziv << ", Duration: " << trajanje << " seconds, Bitrate: " << bitrate << " kbps\n";
  }
};

class VideoFile : public MediaFile {
  int framerate;
public:
  void Play() const override {
    std::cout << "Playing video file: " << naziv << " at resolution: " << " with framerate: " << framerate << " fps\n";
  }
  void PrintInfo() const override {
    std::cout << "Video File: " << naziv << ", Duration: " << trajanje << " seconds, Resolution: " << ", Framerate: " << framerate << " fps\n";
  }
};

class Media {
  MediaFile *p;
public:
  Media(): p(nullptr) {}
  ~Media() { delete p; }
  Media(const MediaFile& b): p(b.DajKopiju()) {}
  void Play() const { p->Play(); }
};

int main() {
  std::vector<Media*> mediji(3, nullptr);
  mediji[0] = new AudioFile("Song1", 240, 320);
  mediji[1] = new VideoFile("Movie1", 7200, "1920x1080", 30);
  mediji[2] = new AudioFile("Song2", 180, 256);
  for(int i = 0; i < mediji.size(); i++)
    mediji[i]->Play();
  return 0;
}
