import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';

import { catchError, retry } from 'rxjs/operators';
import { Observable } from 'rxjs';

@Injectable()
export class GraphAppService {
    constructor(private http: HttpClient) { }
    
    SERVER_ADRESS = '/api/getNodeRelationsFromAI';

    getData(): Observable<any> {
        const httpOptions = {
            headers: new HttpHeaders({
                'Content-Type': 'application/json'
            }),
            timeout: 600000 // 5 seconds timeout
        };

        return this.http.get(`${this.SERVER_ADRESS}`, httpOptions)
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