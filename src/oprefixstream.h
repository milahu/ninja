// https://stackoverflow.com/a/27336473/10440128
// https://github.com/milahu/random/blob/master/cc/print-lines-with-prefix/main.cc

#ifndef NINJA_OPREFIXSTREAM_H_
#define NINJA_OPREFIXSTREAM_H_

#include <iostream>

class prefixbuf
    : public std::streambuf
{
    std::string     prefix;
    std::streambuf* sbuf;
    bool            need_prefix;

    int sync() {
        return this->sbuf->pubsync();
    }
    int overflow(int c) {
        if (c != std::char_traits<char>::eof()) {
            if (this->need_prefix
                && !this->prefix.empty()
                && this->prefix.size() != (long unsigned int) this->sbuf->sputn(&this->prefix[0], this->prefix.size())) {
                return std::char_traits<char>::eof();
            }
            this->need_prefix = c == '\n';
        }
        return this->sbuf->sputc(c);
    }
public:
    prefixbuf(std::string const& prefix, std::streambuf* sbuf)
        : prefix(prefix)
        , sbuf(sbuf)
        , need_prefix(true) {
    }
};

class oprefixstream
    : private virtual prefixbuf
    , public std::ostream
{
public:
    oprefixstream(std::string const& prefix, std::ostream& out)
        : prefixbuf(prefix, out.rdbuf())
        , std::ios(static_cast<std::streambuf*>(this))
        , std::ostream(static_cast<std::streambuf*>(this)) {
    }
};

/*
demo:

int main()
{
    oprefixstream out("prefix: ", std::cout);
    out << "hello\n"
        << "world\n";
}
*/

#endif // NINJA_OPREFIXSTREAM_H_
