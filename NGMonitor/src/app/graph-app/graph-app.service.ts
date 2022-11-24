import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';

import { catchError, retry } from 'rxjs/operators';
import { Observable } from 'rxjs';

@Injectable()
export class GraphAppService {
    constructor(private http: HttpClient) { }
    
    SERVER_ADRESS = '/api/bindings?requestedBinding=NRNCS';

    getData(): Observable<any> {
        return this.http.get(`${this.SERVER_ADRESS}`)
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