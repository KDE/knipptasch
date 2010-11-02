/*
 * Copyright 2010 by Stefan Böhmann <kde@hilefoks.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define STRIGI_IMPORT_API
#include <strigi/analyzerplugin.h>
#include <strigi/streamendanalyzer.h>
#include <strigi/analysisresult.h>
#include <strigi/fieldtypes.h>

#include <cstring>
#include <iostream>


using namespace Strigi;
using namespace std;

class KnipptaschAnalyzerFactory;

class STRIGI_PLUGIN_API KnipptaschAnalyzer : public Strigi::StreamEndAnalyzer
{
    public:
        KnipptaschAnalyzer(const KnipptaschAnalyzerFactory* f)
          : m_factory( f )
        {
        }

        const char* name() const
        {
            return "KnipptaschAnalyzer";
        }


        bool checkHeader(const char* header, int32_t headersize) const;

        bool checkHeader(const char* header, int32_t headerSize,
                         uint32_t *magic, uint32_t *version) const;

        signed char analyze(Strigi::AnalysisResult& idx, Strigi::InputStream* in);

    private:
        const KnipptaschAnalyzerFactory *m_factory;
};


class STRIGI_PLUGIN_API KnipptaschAnalyzerFactory : public StreamEndAnalyzerFactory
{
    friend class KnipptaschAnalyzer;

    private:
        const char* name() const
        {
            return "KnipptaschAnalyzer";
        }

        StreamEndAnalyzer* newInstance() const
        {
            return new KnipptaschAnalyzer( this );
        }

        void registerFields(FieldRegister &r);

    private:
        const RegisteredField* m_fileVersionField;
        const RegisteredField* m_commentField;
        const RegisteredField* m_countField;
};



void KnipptaschAnalyzerFactory::registerFields(FieldRegister &r)
{
    //nfo:isPasswordProtected
    //nfo:count

    m_fileVersionField = r.registerField( "file_version" );
    m_commentField = r.registerField( "http://www.semanticdesktop.org/ontologies/2007/01/19/nie#comment" );
    m_countField = r.registerField( "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#count" );

    addField( m_fileVersionField );
    addField( m_commentField );
    addField( m_countField );
}


bool KnipptaschAnalyzer::checkHeader(const char* header, int32_t headerSize) const
{
    uint32_t magic;
    uint32_t version;

    return checkHeader( header, headerSize, &magic, &version );
}


bool KnipptaschAnalyzer::checkHeader(const char* header, int32_t headerSize,
                                     uint32_t *magic, uint32_t *version) const
{
    do
    {
        if( headerSize < 8 ) {
            break;
        }

        const uint32_t m = 0x0141886C;
        *magic = ( static_cast<unsigned char>( header[ 3 ] ) ) +
                 ( static_cast<unsigned char>( header[ 2 ] ) <<  8 ) +
                 ( static_cast<unsigned char>( header[ 1 ] ) << 16 ) +
                 ( static_cast<unsigned char>( header[ 0 ] ) << 24 );

        if( m != *magic ) {
            break;
        }

        const uint32_t v = 0x000002;
        *version = ( static_cast<unsigned char>( header[ 7 ] ) ) +
                   ( static_cast<unsigned char>( header[ 6 ] ) <<  8 ) +
                   ( static_cast<unsigned char>( header[ 5 ] ) << 16 ) +
                   ( static_cast<unsigned char>( header[ 4 ] ) << 24 );

        if( v != *version ) {
            break;
        }

        return true;
    }
    while( false );

    return false;
}


signed char KnipptaschAnalyzer::analyze(AnalysisResult& idx, InputStream* in)
{
    {
        const char *header;
        int32_t n = in->read( header, 8, 8 );
        in->reset( 0 );

        uint32_t version, magic;
        if( n < 8 || !checkHeader( header, n, &magic, &version ) ) {
            return Error;
        }

        idx.addValue( m_factory->m_fileVersionField, version );
    }

    const char *sc;
    in->skip( 8 );
    {
        int32_t n = in->read( sc, 4, 4 );
        in->reset( 0 );

        if( n < 4 ) {
            return Error;
        }
    }
    const int32_t size = ( static_cast<unsigned char>( sc[ 3 ] ) ) +
                         ( static_cast<unsigned char>( sc[ 2 ] ) <<  8 ) +
                         ( static_cast<unsigned char>( sc[ 1 ] ) << 16 ) +
                         ( static_cast<unsigned char>( sc[ 0 ] ) << 24 );

    std::string str;
    {
        const char *data;
        in->skip( 12 );
        int32_t n = in->read( data, size, size );
        in->reset( 0 );

        if( n < size ) {
            return Error;
        }

        str.append( data, size );
    }
    idx.addValue( m_factory->m_commentField, str );

    return Ok;
}



class Factory : public AnalyzerFactoryFactory
{
    public:
        list<StreamEndAnalyzerFactory*> streamEndAnalyzerFactories() const
        {
            list<StreamEndAnalyzerFactory*> af;
            af.push_back( new KnipptaschAnalyzerFactory() );

            return af;
        }
};


STRIGI_ANALYZER_FACTORY(Factory)

// kate: word-wrap off; encoding utf-8; indent-width 4; tab-width 4; line-numbers on; mixed-indent off; remove-trailing-space-save on; replace-tabs-save on; replace-tabs on; space-indent on;
// vim:set spell et sw=4 ts=4 nowrap cino=l1,cs,U1:
