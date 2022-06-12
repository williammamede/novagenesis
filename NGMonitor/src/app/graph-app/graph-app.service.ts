import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';

import { catchError, retry } from 'rxjs/operators';
import { Observable } from 'rxjs';

@Injectable()
export class GraphAppService {
    constructor(private http: HttpClient) { }
    
    private url = 'localhost:10102';
    private header = {
        headers: new HttpHeaders({
            'Content-Type': 'application/json',
            'Access-Control-Allow-Origin': '*',
            'Access-Control-Allow-Methods': 'GET, POST, OPTIONS, PUT, PATCH, DELETE',
            'Access-Control-Allow-Headers': 'Origin, Accept, X-Requested-With, Content-Type, Access-Control-Request-Method, Access-Control-Request-Headers',
            'requestedBinding': 'NRNCS'
        })
    };


    getData(): Observable<any> {
        return this.http.get(this.url, this.header)
            .pipe(
                retry(3),
                catchError(this.handleError)
            );
    }

    showData() {
        this.getData().subscribe(data => {
            console.log(data);
        });
    }

    private handleError(error: any) {
        const errMsg = (error.message) ? error.message :
            error.status ? `${error.status} - ${error.statusText}` : 'Server error';
        return errMsg;
    }
}