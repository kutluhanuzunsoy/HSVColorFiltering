#ifndef HSVRANGE_H

class HSVRange {
public:
    HSVRange(int hMin, int hMax, int sMin, int sMax, int vMin, int vMax)
            : hMin(hMin), hMax(hMax), sMin(sMin), sMax(sMax), vMin(vMin), vMax(vMax) {}

    bool operator!=(const HSVRange &other) const {
        return hMin != other.hMin || hMax != other.hMax || sMin != other.sMin || sMax != other.sMax ||
               vMin != other.vMin || vMax != other.vMax;
    }

    int& getHMin() { return hMin; }
    int& getHMax() { return hMax; }
    int& getSMin() { return sMin; }
    int& getSMax() { return sMax; }
    int& getVMin() { return vMin; }
    int& getVMax() { return vMax; }

private:
    int hMin, hMax, sMin, sMax, vMin, vMax;
};

#endif //HSVRANGE_H
