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

#include <json/json.h>


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
        const RegisteredField* m_fileVersion;

        const RegisteredField* m_passwordProtected;
        const RegisteredField* m_description;
        const RegisteredField* m_name;
        const RegisteredField* m_number;
        const RegisteredField* m_openingDate;
        const RegisteredField* m_openingBalance;
        const RegisteredField* m_iban;
        const RegisteredField* m_owner;
        const RegisteredField* m_institution;
        const RegisteredField* m_bic;

        const RegisteredField* m_count;
};



void KnipptaschAnalyzerFactory::registerFields(FieldRegister &r)
{
    m_fileVersion = r.registerField( "Account File Version", Strigi::FieldRegister::stringType, 1, 0 );
    m_passwordProtected = r.registerField( "Password Protected", Strigi::FieldRegister::binaryType, 1, 0 );
    m_description = r.registerField( "Description", Strigi::FieldRegister::stringType, 1, 0 );
    m_name = r.registerField( "Account Name", Strigi::FieldRegister::stringType, 1, 0 );
    m_number = r.registerField( "Account Number", Strigi::FieldRegister::stringType, 1, 0 );
    m_openingDate = r.registerField( "Opening Date", Strigi::FieldRegister::datetimeType, 1, 0 );
    m_openingBalance = r.registerField( "Opening Date", Strigi::FieldRegister::stringType, 1, 0 );
    m_iban = r.registerField( "IBAN", Strigi::FieldRegister::stringType, 1, 0 );
    m_owner = r.registerField( "Account Owner", Strigi::FieldRegister::stringType, 1, 0 );
    m_institution = r.registerField( "Institution", Strigi::FieldRegister::stringType, 1, 0 );
    m_bic = r.registerField( "BIC", Strigi::FieldRegister::stringType, 1, 0 );
    m_count = r.registerField( "Posting Count", Strigi::FieldRegister::integerType, 1, 0 );

    addField( m_fileVersion );
    addField( m_passwordProtected );
    addField( m_description );
    addField( m_name );
    addField( m_number );
    addField( m_openingDate );
    addField( m_openingBalance );
    addField( m_iban );
    addField( m_owner );
    addField( m_institution );
    addField( m_bic );
    addField( m_count );
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

        idx.addValue( m_factory->m_fileVersion, version );
    }


    int32_t size = 0;
    {
        const char *sc;
        in->skip( 8 );
        {
            int32_t n = in->read( sc, 4, 4 );
            in->reset( 0 );

            if( n < 4 ) {
                return Error;
            }
        }

        size = ( static_cast<unsigned char>( sc[ 3 ] ) ) +
               ( static_cast<unsigned char>( sc[ 2 ] ) <<  8 ) +
               ( static_cast<unsigned char>( sc[ 1 ] ) << 16 ) +
               ( static_cast<unsigned char>( sc[ 0 ] ) << 24 );
    }

    std::string str;
    {
        const char *data;
        in->skip( 12 );
        int32_t n = in->read( data, size, size );
        in->reset( 0 );

        if( n < size ) {
            return Error;
        }
        str = std::string( data, n );
    }

    struct json_object *json_obj;
    json_obj = json_tokener_parse( str.data() );

    {
        struct json_object *obj = json_object_object_get( json_obj, "passwordProtected" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_boolean ) {
                idx.addValue( m_factory->m_passwordProtected, json_object_get_boolean( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "description" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_string ) {
                idx.addValue( m_factory->m_description, json_object_get_string( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "name" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_string ) {
                idx.addValue( m_factory->m_name, json_object_get_string( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "number" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_string ) {
                idx.addValue( m_factory->m_number, json_object_get_string( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "openingDate" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_string ) {
                idx.addValue( m_factory->m_openingDate, json_object_get_string( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "openingBalance" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_double ) {
                idx.addValue( m_factory->m_openingBalance, json_object_get_double( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "iban" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_string ) {
                idx.addValue( m_factory->m_iban, json_object_get_string( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "owner" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_string ) {
                idx.addValue( m_factory->m_owner, json_object_get_string( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "institution" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_string ) {
                idx.addValue( m_factory->m_institution, json_object_get_string( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "bic" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_string ) {
                idx.addValue( m_factory->m_bic, json_object_get_string( obj ) );
            }
        }
    }
    {
        struct json_object *obj = json_object_object_get( json_obj, "count" );
        if( obj ) {
            if( json_object_get_type( obj ) == json_type_int ) {
                idx.addValue( m_factory->m_count, json_object_get_int( obj ) );
            }
        }
    }

    json_object_put( json_obj );

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
