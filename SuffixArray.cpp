#include <iostream>
using namespace std;

class SuffixArray {
private:
    char *text;
    int *suffixArr;
    int length;

public:
    struct suffix {
        int index;
        int rank[2];
    };

    SuffixArray(char *txt) {
        length = 0;
        while (txt[length] != '\0') {
            length++;
        }

        text = new char[length + 1];
        for (int i = 0; i < length; i++) {
            text[i] = txt[i];
        }
        text[length] = '\0';

        suffixArr = nullptr;
    }
    void ConstructUsingPrefixDoubling() {
        suffixArr = buildSuffixArray(text);
    }

    void Print() {
        for (int i = 0; i < length; i++) {
            cout << suffixArr[i] << " ";
        }
        cout << endl;
    }

private:
    static bool cmp(struct suffix a, struct suffix b) {
        return (a.rank[0] == b.rank[0]) ? (a.rank[1] < b.rank[1]) : (a.rank[0] < b.rank[0]);
    }

    static int *buildSuffixArray(char *txt) {
        int n = 0;
        while (txt[n] != '\0') {
            n++;
        }

        struct suffix *suffixes = new suffix[n];

        for (int i = 0; i < n; i++) {
            suffixes[i].index = i;
            suffixes[i].rank[0] = txt[i] - 'a';
            suffixes[i].rank[1] = ((i + 1) < n) ? (txt[i + 1] - 'a') : -1;
        }

        mergeSort(suffixes, 0, n - 1);
        int *index = new int[n];
        for (int k = 2; k < n; k *= 2) {
            int rank = 0;
            int prev_rank = suffixes[0].rank[0];
            suffixes[0].rank[0] = rank;
            index[suffixes[0].index] = 0;
            for (int i = 1; i < n; i++) {
                if (suffixes[i].rank[0] == prev_rank && suffixes[i].rank[1] == suffixes[i - 1].rank[1]) {
                    prev_rank = suffixes[i].rank[0];
                    suffixes[i].rank[0] = rank;
                } else {
                    prev_rank = suffixes[i].rank[0];
                    suffixes[i].rank[0] = ++rank;
                }
                index[suffixes[i].index] = i;
            }

            for (int i = 0; i < n; i++) {
                int nextindex = suffixes[i].index + k;
                suffixes[i].rank[1] = (nextindex < n) ? suffixes[index[nextindex]].rank[0] : -1;
            }
            mergeSort(suffixes, 0, n - 1);
        }

        int *suffixArr = new int[n];
        for (int i = 0; i < n; i++) {
            suffixArr[i] = suffixes[i].index;
        }

        delete[] suffixes;
        delete[] index;
        return suffixArr;
    }

    static void merge(struct suffix arr[], int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        struct suffix *L = new suffix[n1];
        struct suffix *R = new suffix[n2];

        for (int i = 0; i < n1; i++) {
            L[i] = arr[left + i];
        }
        for (int i = 0; i < n2; i++) {
            R[i] = arr[mid + 1 + i];
        }
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (cmp(L[i], R[j])) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
        delete[] L;
        delete[] R;
    }

    static void mergeSort(struct suffix arr[], int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);

            merge(arr, left, mid, right);
        }
    }

};

int main() {
    SuffixArray t("ACGACTACGATAAC$");

    t.ConstructUsingPrefixDoubling();

    t.Print(); // Prints: 14 11 12 0 6 3 9 13 1 7 4 2 8 10 5

    return 0;
}
