#ifndef SUFFIX_ARRAY_CPP
#define SUFFIX_ARRAY_CPP

#include <iostream>

template <typename T>
void merge(T arr[], int left, int right)
{
    int mid = (left + right) / 2;
    int len1 = mid - left + 1;
    int len2 = right - mid;

    T leftPart[len1], rightPart[len2];
    for (int i = 0; i < len1; i++)
    {
        leftPart[i] = arr[i + left];
    }
    for (int i = 0; i < len2; i++)
    {
        rightPart[i] = arr[i + mid + 1];
    }

    int i = 0, j = 0, k = left;
    while (i < len1 && j < len2)
    {
        if (leftPart[i] < rightPart[j])
        {
            arr[k++] = leftPart[i++];
        }
        else
        {
            arr[k++] = rightPart[j++];
        }
    }

    while (i < len1)
    {
        arr[k++] = leftPart[i++];
    }

    while (j < len2)
    {
        arr[k++] = rightPart[j++];
    }
}

template <typename T>
void mergeSort(T arr[], int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, right);
}

class SuffixArray
{
    char *text;
    int *suffixArray;
    int size = 0;

    struct Suffix
    {
        int index;
        int rank1;
        int rank2;

        bool operator<(Suffix s)
        {
            if (rank1 == s.rank1)
            {
                return rank2 < s.rank2;
            }
            return rank1 < s.rank1;
        }
    };

public:
    SuffixArray(const char *input)
    {
        while (input[size] != '\0')
        {
            size++;
        }
        text = new char[size];
        suffixArray = new int[size];
        for (int i = 0; i < size; i++)
        {
            text[i] = input[i];
        }
    }

    void ConstructUsingPrefixDoubling()
    {
        Suffix suffixes[size];
        for (int i = 0; i < size - 1; i++)
        {
            suffixes[i].index = i;
            suffixes[i].rank1 = text[i];
            suffixes[i].rank2 = text[i + 1];
        }
        suffixes[size - 1].index = size - 1;
        suffixes[size - 1].rank1 = text[size - 1];
        suffixes[size - 1].rank2 = -1;

        mergeSort(suffixes, 0, size - 1);
        int suffixIndices[size];
        for (int i = 2; i < size; i *= 2)
        {
            int rank = 0;
            int pervRank = suffixes[0].rank1;
            suffixes[0].rank1 = 0;
            suffixIndices[suffixes[0].index] = 0;
            for (int j = 1; j < size; j++)
            {
                if (suffixes[j].rank1 != pervRank || suffixes[j].rank2 != suffixes[j - 1].rank2)
                {
                    rank++;
                }
                pervRank = suffixes[j].rank1;
                suffixes[j].rank1 = rank;
                suffixIndices[suffixes[j].index] = j;
            }
            for (int j = 0; j < size; j++)
            {
                int next = suffixes[j].index + (i / 2);
                if (next < size)
                {
                    suffixes[j].rank2 = suffixes[suffixIndices[next]].rank1;
                }
                else
                {
                    suffixes[j].rank2 = -1;
                }
            }
            mergeSort(suffixes, 0, size - 1);
        }

        for (int i = 0; i < size; i++)
        {
            suffixArray[i] = suffixes[i].index;
        }
    }

    void Print()
    {
        for (int i = 0; i < size; i++)
        {
            std::cout << suffixArray[i] << ' ';
        }
        std::cout << '\n';
    }
};

#endif