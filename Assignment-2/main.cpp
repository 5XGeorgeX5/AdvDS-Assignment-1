#include "SuffixArray.cpp"

int main()
{
    SuffixArray t("ACGACTACGATAAC$");

    t.ConstructUsingPrefixDoubling();

    t.Print();

    return 0;
}